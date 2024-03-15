module top #(parameter DIVISOR = 50_000_000,
             parameter FILE_NAME = "mem_init.mif",
             parameter ADDR_WIDTH = 6,
             parameter DATA_WIDTH = 16)
            (input clk,
             input [2:0] btn,
             input [9:0] sw,
             output [9:0] led,
             output [27:0] hex);
    
    wire rst_n;
    
    assign rst_n = sw[9];
    
    wire clk_d;
    clk_div #(DIVISOR) clk_div_init(.clk(clk), .rst_n(rst_n), .out(clk_d));
    
    wire mem_we;
    wire [DATA_WIDTH-1:0] mem_out, mem_data;
    wire [ADDR_WIDTH-1:0] mem_addr;
    wire [10:0] out_other;
    wire [ADDR_WIDTH-1:0] pc, sp;
    wire [3:0] out_ones_sp, out_tens_sp, out_ones_pc, out_tens_pc;
    wire [DATA_WIDTH-1:0] in_cpu;
    
    assign in_cpu = {12'h000,sw[3:0]};
    
    cpu #(ADDR_WIDTH, DATA_WIDTH) cpu_init(.clk(clk_d), .rst_n(rst_n), .mem_we(mem_we),.mem_in(mem_out),
    .in(in_cpu), .mem_addr(mem_addr), .mem_data(mem_data),
    .out({out_other,led[4:0]}), .pc(pc), .sp(sp));
    memory #("mem_init.mif", ADDR_WIDTH, DATA_WIDTH) memory_init(.clk(clk_d), .we(mem_we),
    .addr(mem_addr), .data(mem_data), .out(mem_out));
    
    bcd bcd_sp(.in(sp), .out_ones(out_ones_sp), .out_tens(out_tens_sp));
    bcd bcd_pc(.in(pc), .out_ones(out_ones_pc), .out_tens(out_tens_pc));
    
    ssd ssd_sp_tens(.in(out_tens_sp), .out(hex[27:21]));
    ssd ssd_sp_ones(.in(out_ones_sp), .out(hex[20:14]));
    ssd ssd_pc_tens(.in(out_tens_pc), .out(hex[13:7]));
    ssd ssd_pc_ones(.in(out_ones_pc), .out(hex[6:0]));
    
endmodule
