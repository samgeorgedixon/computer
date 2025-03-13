`include "src/cpu/program-counter_16bit.sv"

module CPU(
    
    input clk, r
    
    );

    reg [15:0] bus = 0;

    ProgramCounter16bit pc(0, 1, 1, clk, r, bus, bus);

endmodule
