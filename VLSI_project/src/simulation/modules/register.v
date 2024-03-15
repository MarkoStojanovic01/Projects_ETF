module register(clk,
                rst_n,
                cl,
                ld,
                in,
                inc,
                dec,
                sr,
                ir,
                sl,
                il,
                out);
    
    input clk, rst_n, cl, ld, inc, dec, sr, ir, sl, il;
    input [3:0] in;
    output [3:0] out;
    
    reg [3:0] out_next, out_reg;
    assign out = out_reg;
    
    always @(posedge clk, negedge rst_n)
        if (!rst_n)
            out_reg <= 4'h0;
        else
            out_reg <= out_next;
    
    always @(*) begin
        out_next = out_reg;
        if (cl)
            out_next = 4'h0;
        else if (ld)
            out_next = in;
        else if (inc)
            out_next = out_reg + {{3{1'b0}}, 1'b1};
        else if (dec)
            out_next = out_reg - {{3{1'b0}}, 1'b1};
        else if (sr) begin
            out_next = out_reg >> 1;
            out_next = out_next | {ir,{3{1'b0}}};
        end
            else if (sl) begin
            out_next = out_reg << 1;
            out_next = out_next | {{3{1'b0}},il};
            end
            
            end
            
            endmodule
