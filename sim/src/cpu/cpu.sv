`include "src/core.svh"

`include "src/cpu/control-unit.sv"
`include "src/cpu/register-unit.sv"

`include "src/cpu/decoder.sv"
`include "src/cpu/addr-manager.sv"

`include "src/cpu/registers/flags.sv"
`include "src/cpu/registers/program-counter.sv"

`include "src/exp-units/memory.sv"
`include "src/exp-units/drive.sv"
`include "src/exp-unit-manager.sv"

module CPU(
    
    input logic clk, r,

    inout wire [15:0] bus,
    inout logic [23:0] addr,

    input string romFilePath,
    input string driveFilePath

    );

    // BUS's
    wire [15:0] bus_alu_a;
    wire [15:0] bus_alu_b;
    wire [7:0]  bus_flags;

    // Direct Registers
    logic [15:0] instr_direct;
    logic [15:0] memory_direct;
    logic [7:0]  flags_direct;

    logic [15:0] cs_direct;
    logic [15:0] ds_direct;
    logic [15:0] ss_direct;
    logic [15:0] es_direct;

    // Control Signals
    ControlSignals_if controlSignalsRaw();    // ControlUnit -> Decoder ->
    ControlSignals_if controlSignals();       // Decoder -> Units...

    // Units

    Decoder decoder(instr_direct, controlSignalsRaw, controlSignals);

    ControlUnit controlUnit(clk, r, instr_direct, flags_direct, controlSignalsRaw); // TODO: Finish Instruction Set

    RegisterUnit registerUnit(clk, r, bus, bus_alu_a, bus_alu_b, instr_direct, memory_direct, cs_direct, ds_direct, ss_direct, es_direct, controlSignals);

    Flags flags(clk, r, bus_flags, flags_direct, controlSignals);
    ProgramCounter programCounter(clk, r, bus, bus_alu_a, bus_alu_b, controlSignals);
    // + ALU

    AddrManager addrManager(addr, memory_direct, cs_direct, ds_direct, ss_direct, es_direct, controlSignals);

    // Expansion Units

    ExpansionSignals_if memoryExpansionSignals();
    Memory rom(clk, r, bus, addr, memoryExpansionSignals, romFilePath);

    ExpansionSignals_if driveExpansionSignals();
    Drive_8bx24b drive(clk, r, bus, addr, driveExpansionSignals, driveFilePath);

    ExpansionUnitManager expUnitManger(.controlSignals(controlSignals),
        .expUnit1(memoryExpansionSignals), .expUnit3(driveExpansionSignals));

endmodule
