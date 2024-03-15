module ssd (input [3:0] in,
            output reg [6:0] out);

always @(*) begin
    case (in)
        4'd0:  out = ~7'h3f;
        4'd1:  out = ~7'h06;
        4'd2:  out = ~7'h5b;
        4'd3:  out = ~7'h4f;
        4'd4:  out = ~7'h66;
        4'd5:  out = ~7'h6d;
        4'd6:  out = ~7'h7d;
        4'd7:  out = ~7'h07;
        4'd8:  out = ~7'h7f;
        4'd9:  out = ~7'h6f;
        4'd10: out = ~7'h77;
        4'd11: out = ~7'h7C;
        4'd12: out = ~7'h39;
        4'd13: out = ~7'h5E;
        4'd14: out = ~7'h79;
        4'd15: out = ~7'h71;
        
    endcase
end

endmodule
