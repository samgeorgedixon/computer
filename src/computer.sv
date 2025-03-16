`include "src/cpu/cpu.sv"
`include "src/ram.sv"

module Computer();

    reg clk = 0;
    reg r = 0;

    wire [15:0] bus;
    wire [23:0] addr;

    wire epawe, epaoe;
    wire epbwe, epboe;
    wire epcwe, epcoe;
    wire epdwe, epdoe;

    CPU cpu_a(clk, r, bus, addr, epawe, epaoe, epbwe, epboe, epcwe, epcoe, epdwe, epdoe);
    RAM16B24A ram(clk, r, 0, 0, 0, bus);

    initial begin

        r = 1;
        
        #1000
        r = 0;

    end

    always #500 clk = ~clk; // Cycle of 1 Micro Second

endmodule
