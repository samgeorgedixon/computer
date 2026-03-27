`include "src/core.svh"

`include "src/cpu/decoder.sv"
`include "src/cpu/control-unit.sv"
`include "src/cpu/register-unit.sv"

`include "src/cpu/registers/flags.sv"
`include "src/cpu/registers/program-counter.sv"

`include "src/exp-unit-manager.sv"
`include "src/exp-units/memory.sv"

module CPU(
    
    input logic clk, r,

    inout wire [15:0] bus,
    input wire [23:0] addr

    );

    // BUS's
    wire [15:0] bus_alu_a;
    wire [15:0] bus_alu_b;
    wire [7:0]  bus_flags;

    // Direct Registers
    logic [15:0] instr_direct;
    logic [15:0] memory_direct;
    logic [7:0]  flags_direct;

    // Control Signals
    ControlSignals_if controlSignalsRaw;    // ControlUnit -> Decoder ->
    ControlSignals_if controlSignals;       // Decoder -> Units...

    // Units

    Decoder decoder(controlSignalsRaw, controlSignals);

    ControlUnit controlUnit(clk, r, instr, flags_direct, controlSignalsRaw); // TODO: Finish Instruction Set

    RegisterUnit registerUnit(clk, r, bus, bus_alu_a, bus_alu_b, instr_direct, memory_direct, controlSignals);

    Flags flags(clk, r, bus_flags, flags_direct, controlSignals);
    ProgramCounter programCounter(clk, r, bus, bus_alu_a, bus_alu_b, controlSignals);
    // + ALU

    // Expansion Units

    ExpansionSignals_if memUnitSignals;
    Memory memory(clk, r, bus, addr, memUnitSignals);

    ExpansionUnitManager expUnitManger(clk, r, bus_src_decoded, bus_dest_decoded, memUnitSignals);

endmodule
