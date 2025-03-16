`timescale 1ns/1ns
`include "src/computer.sv"

module ComputerTB;

    import "DPI-C" function void welcome_cpp();

    Computer computer();

    initial begin

        $dumpfile("logs/computer.vcd");
        $dumpvars(0, ComputerTB);

        $display("---");
        $display("Computer Test Running...");
        welcome_cpp();

    end

    initial #100000 $finish; // One Second

endmodule
