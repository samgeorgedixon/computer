`timescale 1ns/1ns
`include "src/cpu/cpu.sv"
`include "src/ram.sv"

module ComputerTB;

    import "DPI-C" function void welcome_cpp();

    reg clk = 0;
    reg r = 0;

    wire [15:0] bus;

    CPU cpu_a(clk, r, bus);
    RAM16B24A ram(clk, r, 0, 0, 0, bus);

    initial begin

        $dumpfile("logs/computer.vcd");
        $dumpvars();

        $display("---");
        $display("Computer Test Running...");
        welcome_cpp();
        
        r = 1;
        
        #1000
        r = 0;

    end

    always #500 clk = ~clk; // Cycle of 1 Micro Second

    initial #100000 $finish; // One Second

endmodule
