`timescale 1us/100ns

`include "src/cpu/cpu.sv"

module Computer();

    logic clk = 0;
    logic r = 1;

    wire [15:0] bus;
    logic [23:0] addr;

    CPU cpu(clk, r, bus, addr, "../bin/software/bios.bin", "../bin/software/gpu_test.bin");

    initial begin

        $display("---");
        $display("Computer Test Running...");

        #1.1
        r = 0;

    end

    always #0.5 clk = ~clk;

endmodule
