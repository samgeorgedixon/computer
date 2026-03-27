`timescale 1us/100ns

`include "src/cpu/cpu.sv"

module Computer();

    reg clk = 0;
    reg r = 0;

    /* verilator lint_off UNOPTFLAT */
    wire [15:0] bus;
    /* verilator lint_on UNOPTFLAT */
    wire [23:0] addr;

    CPU cpu(clk, r, bus, addr);

    initial begin

        $display("---");
        $display("Computer Test Running...");

        r = 1;

        #1.1
        r = 0;

    end

    always #0.5 clk = ~clk;

endmodule
