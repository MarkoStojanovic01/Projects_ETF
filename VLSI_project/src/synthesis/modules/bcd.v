module bcd (input [5:0] in,
            output [3:0] out_ones,
            output [3:0] out_tens);

assign out_ones = in % 10;
assign out_tens = in / 10 % 10;

endmodule
