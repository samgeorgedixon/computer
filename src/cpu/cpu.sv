`include "src/cpu/program_counter.sv"
`include "src/cpu/alu.sv"
`include "src/cpu/reg_16bit.sv"
`include "src/cpu/reg_8bit.sv"
`include "src/cpu/registers_unit.sv"

module CPU(
    
    input clk, r
    
    );

    reg [15:0] bus = 0;

    reg [7:0] flags = 0;

    reg [15:0] reg16AO = 0;

    ProgramCounter pc(clk, r, bus, 0, 0, 1, bus);

    ALU alu(clk, reg16AO, 2, 'b100, 1, bus, flags);

    Reg16 reg16A(clk, r, 1, 0, bus, bus, reg16AO);

    //RegistersUnit regUnit(clk, r);

endmodule
