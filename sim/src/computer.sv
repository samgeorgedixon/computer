`timescale 1us/100ns

`include "src/cpu/cpu.sv"

module Computer(

    input logic clk,
    input logic r,

    output wire [15:0] bus,
    output logic [23:0] addr

    );

    CPU cpu(clk, r, bus, addr, "../bin/software/bios.bin", "../bin/software/gpu_test.bin");

endmodule
