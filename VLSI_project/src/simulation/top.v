module top; reg [3:0] a, b; reg [2:0] oc; wire [3:0] f; integer index; reg clk, rst_n, cl, ld, inc, dec, sr, ir, sl, il; reg [3:0] in; wire [3:0] out; alu aluMod(.oc(oc), .a(a), .b(b), .f(f));
    
    register regMod(.clk(clk), .rst_n(rst_n), .cl(cl), .ld(ld),
    .inc(inc), .dec(dec), .sr(sr), .ir(ir), .sl(sl), .il(il),
    .in(in), .out(out));
    
    initial begin
        for (index = 0; index < 2**11; index = index + 1) begin
            {oc, a, b} = index;
            #5;
        end
        $stop;
        
        rst_n    = 1'b0; clk    = 1'b0; cl    = 1'b0; ld    = 1'b0; inc    = 1'b0; dec    = 1'b0;
        sr       = 1'b0; ir       = 1'b0; sl       = 1'b0; il       = 1'b0;  in       = 8'h00;
        #2 rst_n = 1'b1;
        repeat (1000) begin
            #5;
            {cl, ld, inc, dec, sr, ir, sl, il, in} = $urandom_range(4096);
        end
        #5 $finish;
    end
    
    always
    #5 clk = ~clk;
    
    initial begin
        $monitor("OC = %3b, a = %4b, b = %4b, f = %4b",
        oc, a, b, f);
    end
    
    always @(out) begin
        $strobe("Time = %d, in = %4b, cl = %b, ld = %b, inc = %b, dec = %b, sr = %b, ir = %b, sl = %b, il = %b, out = %4b",
        $time, in, cl, ld, inc, dec, sr, ir, sl, il, out);
    end
    
    
    
endmodule
