`timescale 1us/100ns

`include "src/computer/cpu/cpu.sv"
`include "src/computer/memory_unit.sv"
`include "src/computer/exts/drive_ext.sv"
`include "src/computer/exts/gpu_ext.sv"

module Computer();

    reg clk = 0;
    reg r = 0;

    /* verilator lint_off UNOPTFLAT */
    wire [15:0] bus;
    /* verilator lint_on UNOPTFLAT */
    wire [23:0] addro;

    wire [15:0] addr;
    wire [15:0] saddr;

    wire e1i, e1o;
    wire e2i, e2o;
    wire e3i, e3o;
    wire e4i, e4o;

    wire ri, rbi, ro, rbo;

    CPU cpu(clk, r, bus, addro, addr, saddr, ri, rbi, ro, rbo, e1i, e1o, e2i, e2o, e3i, e3o, e4i, e4o);

    MemoryUnit memoryUnit(clk, r, bus, ri, rbi, ro, rbo, addr, saddr, addro, "../programs/bin/programs/bios.bin");

    DriveExt drive(clk, r, bus, addro, e1i, e1o, "../programs/bin/drives/drive.bin");

    GPU_Ext gpu(clk, r, bus, addro, e2i, e2o);

    initial begin

        $display("---");
        $display("Computer Test Running...");

        r = 1;

        #1.1
        r = 0;

    end

    always #0.5 clk = ~clk;

endmodule
