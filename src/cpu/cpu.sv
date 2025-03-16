`include "src/cpu/program_counter.sv"
`include "src/cpu/alu.sv"
`include "src/cpu/reg_16bit.sv"
`include "src/cpu/reg_8bit.sv"
`include "src/cpu/registers_unit.sv"
`include "src/cpu/control_unit.sv"

module CPU(
    
    input clk, r,
    inout reg [15:0] bus,
    output [23:0] addr,

    output epawe, epaoe,
    output epbwe, epboe,
    output epcwe, epcoe,
    output epdwe, epdoe
    
    );

    reg [7:0] flags = 0;

    wire AI, AO, BI, BO, CI, CO, DI, DO, EI, EO, II, IO, MI, RI, RO, PCE, PCI, PCO, SO, S0, S1, S2, FI, AEI, AEO, BEI, BEO, CEI, CEO, DEI, DEO, SPI, SPO, SBI, SBO, CSI, CSO, DSI, SSI, ESI, SSO0, SSO1, SSO2, SSO, RSBI, RSBO, RSTI, RSTO;

    wire [3:0] rsbi, rsbo, rsti, rsto;

    wire [15:0] ao, bo, co, do_, eo, mo, io, spo, sbo, so;

    ProgramCounter pc(clk, r, bus, PCI, PCO, PCE, bus);

    ALU alu(clk, ao, bo, { S0, S1, S2 }, 1, bus, flags);

    RegistersUnit regUnit(clk, r, bus, rsbi, rsbo, rsti, rsto, AI, AO, BI, BO, CI, CO, DI, DO, EI, EO, CSI, CSO, DSI, SSI, ESI, { SSO0, SSO1, SSO2 }, MI, II, IO, SPI, SPO, SBI, SBO, ao, bo, co, do_, eo, mo, io, spo, sbo, so);

    ControlUnit controlUnit(clk, r, io, flags, rsbi, rsbo, rsti, rsto, AI, AO, BI, BO, CI, CO, DI, DO, EI, EO, II, IO, MI, RI, RO, PCE, PCI, PCO, SO, S0, S1, S2, FI, AEI, AEO, BEI, BEO, CEI, CEO, DEI, DEO, SPI, SPO, SBI, SBO, CSI, CSO, DSI, SSI, ESI, SSO0, SSO1, SSO2, SSO , RSBI, RSBO, RSTI, RSTO);

endmodule
