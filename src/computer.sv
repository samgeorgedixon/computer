`include "src/cpu/cpu.sv"
`include "src/memory_unit.sv"
`include "src/drive_ext.sv"

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

    MemoryUnit memoryUnit(clk, r, bus, rwe, roe, addr, saddr, addro);

    DriveExt drive(clk, r, bus, addro, epawe, epaoe);

    initial begin

        r = 1;
        
        #1100
        r = 0;

    end

    always #500 clk = ~clk; // Cycle of 1 Micro Second

endmodule
