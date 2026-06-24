`timescale 1us/100ns

`include "src/cpu/cpu.sv"

module Computer(

    input logic clk,
    input logic r

    );

    wire [15:0] bus;
    logic [23:0] addr;

    logic int_e, byte_low;

    ExpansionUnitCommon_if xuCommon(clk, r, addr, int_e, byte_low);
    ExpansionUnitSignals_if xu[16]();

    CPU cpu(clk, r, bus, addr, int_e, byte_low, xu);

    Memory       memory (bus, xuCommon, xu[1 - 1], "../bin/software/bios.bin");
    Drive_8bx24b drive  (bus, xuCommon, xu[2 - 1], "../bin/software/gpu_test.bin");
    GPU          gpu    (bus, xuCommon, xu[3 - 1]);

endmodule
