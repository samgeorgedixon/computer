`timescale 1ns/1ns
`include "src/cpu.sv"

module computer_tb;

    import "DPI-C" function void welcome_cpp();

    cpu cpu_a();

    initial begin

        welcome_cpp();
        
        $display("Finished");

    end

endmodule
