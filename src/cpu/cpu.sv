`include "src/cpu/program_counter.sv"
`include "src/cpu/alu.sv"
`include "src/cpu/reg_16bit.sv"
`include "src/cpu/reg_8bit.sv"
`include "src/cpu/registers_unit.sv"
`include "src/cpu/control_unit.sv"

module CPU(
    
    input clk, r,
    inout [15:0] bus,

    input [23:0] addro,
    output [15:0] addr,
    output [15:0] saddr,

    output rwe, roe,

    output epawe, epaoe,
    output epbwe, epboe,
    output epcwe, epcoe,
    output epdwe, epdoe
    
    );

    wire AI, AO, BI, BO, CI, CO, DI, DO, EI, EO, II, IO, MI, RI, RO, PCE, PCI, PCO, SO, S0, S1, S2, FI, AEI, AEO, BEI, BEO, CEI, CEO, DEI, DEO, SPI, SPO, SBI, SBO, CSI, CSO, DSI, SSI, ESI, SSO0, SSO1, SSO2, SSO, RSBI, RSBO, RSTI, RSTO;

    wire [3:0] rsbi, rsbo, rsti, rsto;
    wire [15:0] ao, bo, co, do_, eo, mo, io, spo, sbo, so;
    
    assign rwe = RI;
    assign roe = RO;

    assign epawe = AEI; assign epaoe = AEO;
    assign epbwe = BEI; assign epboe = BEO;
    assign epcwe = CEI; assign epcoe = CEO;
    assign epdwe = DEI; assign epdoe = DEO;

    assign addr = mo;
    assign saddr = so;

    wire [7:0] flagsIn;
    wire [7:0] flagsOut;

    ProgramCounter pc(clk, r, bus, PCI, PCO, PCE, bus);

    Reg8 flags(clk, r, FI, 0, flagsIn, flagsOut, flagsOut);
    ALU alu(clk, ao, bo, { S2, S1, S0 }, SO, bus, flagsIn);

    RegistersUnit regUnit(clk, r, bus, rsbi, rsbo, rsti, rsto, AI, AO, BI, BO, CI, CO, DI, DO, EI, EO, CSI, CSO, DSI, SSI, ESI, { SSO2, SSO1, SSO0 }, MI, II, IO, SPI, SPO, SBI, SBO, ao, bo, co, do_, eo, mo, io, spo, sbo, so);

    ControlUnit controlUnit(clk, r, io, flagsOut, rsbi, rsbo, rsti, rsto, AI, AO, BI, BO, CI, CO, DI, DO, EI, EO, II, IO, MI, RI, RO, PCE, PCI, PCO, SO, S0, S1, S2, FI, AEI, AEO, BEI, BEO, CEI, CEO, DEI, DEO, SPI, SPO, SBI, SBO, CSI, CSO, DSI, SSI, ESI, SSO0, SSO1, SSO2, SSO, RSBI, RSBO, RSTI, RSTO);

endmodule
