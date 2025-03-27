`include "src/computer/cpu/program_counter.sv"
`include "src/computer/cpu/alu.sv"
`include "src/computer/cpu/reg_16bit.sv"
`include "src/computer/cpu/reg_8bit.sv"
`include "src/computer/cpu/registers_unit.sv"
`include "src/computer/cpu/control_unit.sv"

module CPU(
    
    input clk, r,
    inout [15:0] bus,

    input [23:0] addro,
    output [15:0] addr,
    output [15:0] saddr,

    output ri, ro,

    output e1i, e1o,
    output e2i, e2o,
    output e3i, e3o,
    output e4i, e4o
    
    );

    wire R1I, R1O, R2I, R2O, R3I, R3O, R4I, R4O, SPI, SPO, SBI, SBO, CSI, CSO, DSI, DSO, SSI, SSO, ESI, ESO, II, IO, MI, MO, PCE, PCI, PCO, SO, FI, OS1I, OS1O, OS1A, OS2I, OS2O, OS2B, OSLI, OSLO;

    wire [3:0] rsi, rso;
    wire [3:0] aluOpSel, aluASel, aluBSel;
    wire [2:0] segSel;
    
    wire [15:0] r1o, r2o, r3o, r4o, spo, bpo, cso, sso, dso, eso, so, mo, io, pco;
    
    assign addr = mo;
    assign saddr = so;

    wire [7:0] flagsIn, flagsOut;
    Reg8 flags(clk, r, FI, 0, flagsIn, flagsOut, flagsOut);

    ProgramCounter pc(clk, r, bus, PCI, PCO, PCE, pco);

    ALU alu(clk, bus, aluOpSel, aluASel, aluBSel, SO, r1o, r2o, r3o, r4o, spo, bpo, cso, sso, dso, eso, mo, pco, flagsIn);

    RegistersUnit regUnit(clk, r, bus, rsi, rso, R1I, R1O, R2I, R2O, R3I, R3O, R4I, R4O, CSI, CSO, DSI, DSO, SSI, SSO, ESI, ESO, segSel, II, IO, MI, MO, SPI, SPO, SBI, SBO, r1o, r2o, r3o, r4o, mo, io, spo, bpo, cso, sso, dso, eso, so);

    ControlUnit controlUnit(clk, r, io, flagsOut, rsi, rso, aluOpSel, aluASel, aluBSel, segSel,
            R1I, R1O, R2I, R2O, R3I, R3O, R4I, R4O, SPI, SPO, SBI, SBO, CSI, CSO, DSI, DSO, SSI, SSO, ESI, ESO, II, IO, MI, MO, ri, ro, PCE, PCI, PCO, SO, FI, OS1I, OS1O, OS1A, OS2I, OS2O, OS2B, OSLI, OSLO, e1i, e1o, e2i, e2o, e3i, e3o, e4i, e4o,
            "bin/cpu_instr_gen/cpu_instr_rom.bin");

endmodule
