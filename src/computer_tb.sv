`timescale 1ns/1ns
`include "src/cpu/cpu.sv"

module ComputerTB;

    import "DPI-C" function void welcome_cpp();

    reg clk = 0;
    reg r = 0;

    CPU cpu_a(clk, r);

    initial begin

        $dumpfile("logs/computer.vcd");
        $dumpvars();

        $display("---");
        $display("Computer Test Running...");
        welcome_cpp();
        
        #5 r = 1;
        #20 r = 0;

    end

    always #10 clk = ~clk;

    initial #10000 $finish;

endmodule
