module cpu #(parameter ADDR_WIDTH = 6,
             parameter DATA_WIDTH = 16)
            (input clk,
             input rst_n,
             output reg mem_we,
             input [DATA_WIDTH - 1:0] mem_in,
             input [DATA_WIDTH - 1:0] in,
             output reg [ADDR_WIDTH - 1:0] mem_addr,
             output reg [DATA_WIDTH - 1:0] mem_data,
             output [DATA_WIDTH - 1:0] out,
             output [ADDR_WIDTH - 1:0] pc,
             output [ADDR_WIDTH - 1:0] sp);
    
    localparam START             = 5'h1F;
    localparam FETCH             = 5'h00;
    localparam DECODE            = 5'h01;
    localparam MOVE_VAR_1        = 5'h02;
    localparam MOVE_VAR_INDIR1   = 5'h03;
    localparam MOVE_VAR_INDIR2   = 5'h04;
    localparam MOVE_VAR_2        = 5'h05;
    localparam MOVE_CONST_1      = 5'h07;
    localparam MOVE_CONST_2      = 5'h08;
    localparam MOVE_CONST_INDIR  = 5'h09;
    localparam IN                = 5'h0B;
    localparam IN_INDIR          = 5'h0C;
    localparam OUT_1             = 5'h0D;
    localparam OUT_INDIR         = 5'h0E;
    localparam OUT_2             = 5'h0F;
    localparam GET_OPRNDS_1      = 5'h10;
    localparam GET_OPRNDS_INDIR2 = 5'h11;
    localparam GET_OPRNDS_2      = 5'h12;
    localparam GET_OPRNDS_INDIR3 = 5'h13;
    localparam EXECUTE_1         = 5'h14;
    localparam EXECUTE_2         = 5'h15;
    localparam STOP_1            = 5'h16;
    localparam STOP_1_INDIR      = 5'h17;
    localparam STOP_2            = 5'h18;
    localparam STOP_2_INDIR      = 5'h19;
    localparam STOP_3            = 5'h1A;
    localparam STOP_3_INDIR      = 5'h1B;
    localparam STOP_4            = 5'h1C;
    
    
    
    
    
    reg [DATA_WIDTH - 1:0] out_reg, out_next;
    reg [4:0] state_reg, state_next;
    
    assign out = out_reg;
    
    reg pcCl, pcLd, pcInc, pcDec, pcSr, pcIr, pcSl, pcIl;
    reg [ADDR_WIDTH-1:0] pcIn;
    
    reg spCl, spLd, spInc, spDec, spSr, spIr, spSl, spIl;
    reg [ADDR_WIDTH-1:0] spIn;
    
    reg irCl, irLd, irInc, irDec, irSr, irIr, irSl, irIl;
    reg [(DATA_WIDTH<<1)-1:0] irIn;
    wire [(DATA_WIDTH<<1)-1:0] irOut;
    
    reg acCl, acLd, acInc, acDec, acSr, acIr, acSl, acIl;
    reg [DATA_WIDTH-1:0] acIn;
    wire [DATA_WIDTH-1:0] acOut;
    
    reg [2:0] oc;
    reg [DATA_WIDTH-1:0] a, b;
    wire [DATA_WIDTH-1:0] aluOut;
    
    // init Program Counter register
    register #(ADDR_WIDTH) regPC(.clk(clk), .rst_n(rst_n), .cl(pcCl), .ld(pcLd),
    .inc(pcInc), .dec(pcDec), .sr(pcSr), .ir(pcIr), .sl(pcSl), .il(pcIl),
    .in(pcIn), .out(pc));
    
    // init Stack Pointer register
    register #(ADDR_WIDTH) regSP(.clk(clk), .rst_n(rst_n), .cl(spCl), .ld(spLd),
    .inc(spInc), .dec(spDec), .sr(spSr), .ir(spIr), .sl(spSl), .il(spIl),
    .in(spIn), .out(sp));
    
    // init Instruction register - WIDTH = 2 Memory Words
    register #(DATA_WIDTH<<1) regIR(.clk(clk), .rst_n(rst_n), .cl(irCl), .ld(irLd),
    .inc(irInc), .dec(irDec), .sr(irSr), .ir(irIr), .sl(irSl), .il(irIl),
    .in(irIn), .out(irOut));
    
    // init Accumulator - WIDTH = 1 Memory Words
    register #(DATA_WIDTH) regAC(.clk(clk), .rst_n(rst_n), .cl(acCl), .ld(acLd),
    .inc(acInc), .dec(acDec), .sr(acSr), .ir(acIr), .sl(acSl), .il(acIl),
    .in(acIn), .out(acOut));
    
    // init ALU
    alu #(DATA_WIDTH) aluInit(.oc(oc), .a(a), .b(b), .f(aluOut));
    
    always @(posedge clk, negedge rst_n) begin
        if (!rst_n) begin
            state_reg <= START;
            out_reg   <= 16'h0000;
        end
        else begin
            out_reg   <= out_next;
            state_reg <= state_next;
        end
    end
    
    always @(*) begin
        out_next   = out_reg;
        state_next = state_reg;
        // clearing everything else
        mem_we   = 1'b0;
        mem_addr = 6'h00;
        mem_data = 16'h0000;
        oc       = 3'b000;
        a        = 16'h0000;
        b        = 16'h0000;
        // PC
        pcIn  = 6'h00;
        pcLd  = 1'b0;
        pcCl  = 1'b0;
        pcInc = 1'b0;
        pcDec = 1'b0;
        pcSr  = 1'b0;
        pcIr  = 1'b0;
        pcSl  = 1'b0;
        pcIl  = 1'b0;
        // SP
        spIn  = 6'h00;
        spLd  = 1'b0;
        spCl  = 1'b0;
        spInc = 1'b0;
        spDec = 1'b0;
        spSr  = 1'b0;
        spIr  = 1'b0;
        spSl  = 1'b0;
        spIl  = 1'b0;
        // IR
        irIn  = 32'h00000000;
        irCl  = 1'b0;
        irLd  = 1'b0;
        irInc = 1'b0;
        irDec = 1'b0;
        irSr  = 1'b0;
        irIr  = 1'b0;
        irSl  = 1'b0;
        irIl  = 1'b0;
        // AC
        acIn  = 16'h0000;
        acCl  = 1'b0;
        acLd  = 1'b0;
        acInc = 1'b0;
        acDec = 1'b0;
        acSr  = 1'b0;
        acIr  = 1'b0;
        acSl  = 1'b0;
        acIl  = 1'b0;
        
        case (state_reg)
            START: begin
                state_next = FETCH; // instruction fetch
                pcIn       = 6'h08; // insructions start addr
                pcLd       = 1'b1;
                spIn       = 6'h3F; // stack start addr, from top to bottom
                spLd       = 1'b1;
            end
            FETCH: begin
                mem_addr   = pc;
                mem_we     = 1'b0;
                pcInc      = 1'b1;
                state_next = DECODE;
            end
            DECODE: begin
                pcInc = 1'b0;
                irIn  = {mem_in,16'h0000}; // first loading higher 16 bits
                irLd  = 1'b1;
                case (mem_in[15:12])
                    4'h0: begin
                        // Move
                        case(mem_in[3:0])
                            4'h0: begin
                                state_next = MOVE_VAR_1;
                            end
                            4'h8: begin
                                state_next = MOVE_CONST_1;
                            end
                            default: begin
                                // unregular instruction
                                state_next = FETCH;
                            end
                        endcase
                    end
                    4'h1: begin
                        // Add
                        state_next = GET_OPRNDS_1;
                    end
                    4'h2: begin
                        // Sub
                        state_next = GET_OPRNDS_1;
                    end
                    4'h3: begin
                        // Mul
                        state_next = GET_OPRNDS_1;
                    end
                    4'h4: begin
                        // Div
                        state_next = GET_OPRNDS_1;
                    end
                    4'h7: begin
                        // Input
                        state_next = IN;
                    end
                    4'h8: begin
                        // Output
                        state_next = OUT_1;
                    end
                    4'hF: begin
                        // Stop
                        state_next = STOP_1;
                    end
                    default: begin
                        // Stop
                        state_next = STOP_1;
                    end
                endcase
            end
            MOVE_VAR_1: begin
                // getting second operand
                mem_addr = {3'b000, irOut[22:20]};
                mem_we   = 1'b0;
                // checking if its direct or indirect addr
                if (irOut[23])
                    state_next = MOVE_VAR_INDIR2;
                else
                    state_next = MOVE_VAR_2;
            end
            MOVE_VAR_INDIR2: begin
                mem_addr   = mem_in[ADDR_WIDTH - 1:0];
                mem_we     = 1'b0;
                state_next = MOVE_VAR_2;
            end
            MOVE_VAR_2: begin
                // checking if its direct or indirect addr
                if (irOut[27]) begin
                    // saving second operand in AC
                    acIn = mem_in;
                    acLd = 1'b1;
                    // getting first operand
                    mem_addr   = {3'b000, irOut[26:24]};
                    mem_we     = 1'b0;
                    state_next = MOVE_VAR_INDIR1;
                end
                else begin
                    // mem write
                    mem_addr   = {3'b000, irOut[26:24]};
                    mem_we     = 1'b1;
                    mem_data   = mem_in;
                    state_next = FETCH;
                end
            end
            MOVE_VAR_INDIR1: begin
                // mem write
                mem_addr   = mem_in[ADDR_WIDTH - 1:0];
                mem_we     = 1'b1;
                mem_data   = acOut;
                state_next = FETCH;
            end
            MOVE_CONST_1: begin
                // getting second word
                mem_addr   = pc;
                mem_we     = 1'b0;
                pcInc      = 1'b1;
                state_next = MOVE_CONST_2;
            end
            MOVE_CONST_2: begin
                pcInc = 1'b0;
                irIn  = {irOut[31:16],mem_in}; // loading lower 16 bits
                irLd  = 1'b1;
                // checking if its direct or indirect addr
                if (irOut[27]) begin
                    // getting first operand
                    mem_addr   = {3'b000, irOut[26:24]};
                    mem_we     = 1'b0;
                    state_next = MOVE_CONST_INDIR;
                end
                else begin
                    // mem write
                    mem_addr   = {3'b000, irOut[26:24]};
                    mem_we     = 1'b1;
                    mem_data   = mem_in;
                    state_next = FETCH;
                end
            end
            MOVE_CONST_INDIR: begin
                // mem write
                mem_addr   = mem_in[ADDR_WIDTH - 1:0];
                mem_we     = 1'b1;
                mem_data   = irOut[15:0];
                state_next = FETCH;
            end
            IN: begin
                // checking if its direct or indirect addr
                if (irOut[27]) begin
                    // getting first operand
                    mem_addr   = {3'b000, irOut[26:24]};
                    mem_we     = 1'b0;
                    state_next = IN_INDIR;
                end
                else begin
                    // mem write
                    mem_addr   = {3'b000, irOut[26:24]};
                    mem_we     = 1'b1;
                    mem_data   = in;
                    state_next = FETCH;
                end
            end
            IN_INDIR: begin
                // mem write
                mem_addr   = mem_in[ADDR_WIDTH - 1:0];
                mem_we     = 1'b1;
                mem_data   = in;
                state_next = FETCH;
            end
            OUT_1: begin
                // getting first operand
                mem_addr = {3'b000, irOut[26:24]};
                mem_we   = 1'b0;
                // checking if its direct or indirect addr
                if (irOut[27])
                    state_next = OUT_INDIR;
                else
                    state_next = OUT_2;
            end
            OUT_INDIR: begin
                mem_addr   = mem_in[ADDR_WIDTH - 1:0];
                mem_we     = 1'b0;
                state_next = OUT_2;
            end
            OUT_2: begin
                out_next   = mem_in;
                state_next = FETCH;
            end
            GET_OPRNDS_1: begin
                // getting second operand
                mem_addr = {3'b000, irOut[22:20]};
                mem_we   = 1'b0;
                // checking if its direct or indirect addr
                if (irOut[23])
                    state_next = GET_OPRNDS_INDIR2;
                else
                    state_next = GET_OPRNDS_2;
            end
            GET_OPRNDS_INDIR2: begin
                mem_addr   = mem_in[ADDR_WIDTH - 1:0];
                mem_we     = 1'b0;
                state_next = GET_OPRNDS_2;
            end
            GET_OPRNDS_2: begin
                // saving second operand in AC
                acIn = mem_in;
                acLd = 1'b1;
                // getting third operand
                mem_addr = {3'b000, irOut[18:16]};
                mem_we   = 1'b0;
                // checking if its direct or indirect addr
                if (irOut[19])
                    state_next = GET_OPRNDS_INDIR3;
                else
                    state_next = EXECUTE_1;
            end
            GET_OPRNDS_INDIR3: begin
                mem_addr   = mem_in[ADDR_WIDTH - 1:0];
                mem_we     = 1'b0;
                state_next = EXECUTE_1;
            end
            EXECUTE_1: begin
                a = acOut;
                b = mem_in;
                case (irOut[31:28])
                    4'h1: begin
                        // Add
                        oc = 3'b000;
                    end
                    4'h2: begin
                        // Sub
                        oc = 3'b001;
                    end
                    4'h3: begin
                        // Mul
                        oc = 3'b010;
                    end
                    4'h4: begin
                        // Div
                        oc = 3'b011;
                    end
                endcase
                // saving value in AC
                acIn = aluOut;
                acLd = 1'b1;
                // checking if its direct or indirect addr for first operand
                if (irOut[27]) begin
                    mem_addr = {3'b000, irOut[26:24]};
                    mem_we   = 1'b0;
                end
                state_next = EXECUTE_2;
            end
            EXECUTE_2: begin
                if (irOut[27]) begin
                    mem_addr = mem_in[ADDR_WIDTH - 1:0];
                end
                else begin
                    mem_addr = {3'b000, irOut[26:24]};
                end
                mem_we     = 1'b1;
                mem_data   = acOut;
                state_next = FETCH;
            end
            STOP_1: begin
                if (irOut[26:24] == 3'b000) begin
                    state_next = STOP_2;
                end
                else begin
                    // getting first operand
                    mem_addr = {3'b000, irOut[26:24]};
                    mem_we   = 1'b0;
                    // checking if its direct or indirect addr
                    if (irOut[27])
                        state_next = STOP_1_INDIR;
                    else
                        state_next = STOP_2;
                end
            end
            STOP_1_INDIR: begin
                mem_addr   = mem_in[ADDR_WIDTH - 1:0];
                mem_we     = 1'b0;
                state_next = STOP_2;
            end
            STOP_2: begin
                // display first operand
                if (!(irOut[26:24] == 3'b000)) begin
                    out_next = mem_in;
                end
                if (irOut[22:20] == 3'b000) begin
                    state_next = STOP_3;
                end
                else begin
                    // getting second operand
                    mem_addr = {3'b000, irOut[22:20]};
                    mem_we   = 1'b0;
                    // checking if its direct or indirect addr
                    if (irOut[23])
                        state_next = STOP_2_INDIR;
                    else
                        state_next = STOP_3;
                end
            end
            STOP_2_INDIR: begin
                mem_addr   = mem_in[ADDR_WIDTH - 1:0];
                mem_we     = 1'b0;
                state_next = STOP_3;
            end
            STOP_3: begin
                // display second operand
                if (!(irOut[22:20] == 3'b000)) begin
                    out_next = mem_in;
                end
                // getting third operand
                mem_addr = {3'b000, irOut[18:16]};
                mem_we   = 1'b0;
                // checking if its direct or indirect addr
                if (irOut[19])
                    state_next = STOP_3_INDIR;
                else
                    state_next = STOP_4;
            end
            STOP_3_INDIR: begin
                mem_addr   = mem_in[ADDR_WIDTH - 1:0];
                mem_we     = 1'b0;
                state_next = STOP_4;
            end
            STOP_4: begin
                // display third operand
                if (!(irOut[18:16] == 3'b000)) begin
                    out_next = mem_in;
                end
                
            end
        endcase
    end
    
endmodule
