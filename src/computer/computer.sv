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

    wire epawe, epaoe;
    wire epbwe, epboe;
    wire epcwe, epcoe;
    wire epdwe, epdoe;

    wire rwe, roe;

    CPU cpu(clk, r, bus, addro, addr, saddr, rwe, roe, epawe, epaoe, epbwe, epboe, epcwe, epcoe, epdwe, epdoe);

    MemoryUnit memoryUnit(clk, r, bus, rwe, roe, addr, saddr, addro, "bin/programs/bios.bin");

    DriveExt drive(clk, r, bus, addro, epawe, epaoe, "bin/programs/text_editor.bin");

    GPU_Ext gpu(clk, r, bus, addro, epbwe, epboe);

    initial begin

        $display("---");
        $display("Computer Test Running...");

        r = 1;

        #1.1
        r = 0;

    end

    always #0.5 clk = ~clk;

endmodule
