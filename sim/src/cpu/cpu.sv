`include "src/core.svh"

`include "src/cpu/control-unit.sv"
`include "src/cpu/register-unit.sv"
`include "src/cpu/alu.sv"

`include "src/cpu/decoder.sv"
`include "src/cpu/addr-manager.sv"

`include "src/cpu/registers/flags.sv"
`include "src/cpu/registers/instruction-pointer-unit.sv"

`include "src/expansion-units/memory.sv"
`include "src/expansion-units/drive.sv"
`include "src/expansion-units/gpu.sv"
`include "src/expansion-unit-manager.sv"

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
    logic [7:0]  bus_flags;

    // Direct Registers
    logic [15:0] ir_direct;
    logic [15:0] ar_direct;
    logic [7:0]  flags_direct;

    logic [15:0] cs_direct;
    logic [15:0] ds_direct;
    logic [15:0] ss_direct;
    logic [15:0] es_direct;

    // Control Signals
    ControlSignals_if controlSignalsRaw();    // ControlUnit -> Decoder ->
    ControlSignals_if controlSignals();       // Decoder -> Units...

    // Units

    ControlUnit controlUnit(clk, r, ir_direct, flags_direct, controlSignalsRaw);

    RegisterUnit registerUnit(clk, r, bus, bus_alu_a, bus_alu_b, ir_direct, ar_direct, cs_direct, ds_direct, ss_direct, es_direct, controlSignals);

    ALU alu(r, bus, bus_alu_a, bus_alu_b, bus_flags, controlSignals);

    InstructionPointerUnit programCounter(clk, r, bus, bus_alu_a, bus_alu_b, controlSignals);
    
    AddrManager addrManager(addr, ar_direct, cs_direct, ds_direct, ss_direct, es_direct, controlSignals);
    Decoder decoder(ir_direct, controlSignalsRaw, controlSignals);

    Flags flags(clk, r, bus_flags, flags_direct, controlSignals);

    // Expansion Units

    ExpansionSignals_if memoryExpansionSignals();
    Memory          rom(clk, r, controlSignals.byte_low, bus, addr, memoryExpansionSignals, romFilePath);

    ExpansionSignals_if driveExpansionSignals();
    Drive_8bx24b    drive(clk, r, controlSignals.byte_low, bus, addr, driveExpansionSignals, driveFilePath);

    ExpansionSignals_if gpuExpansionSignals();
    GPU             gpu(clk, r, controlSignals.byte_low, bus, addr, gpuExpansionSignals);

    ExpansionUnitManager expUnitManger(.controlSignals(controlSignals),
        .xu1(memoryExpansionSignals), .xu2(driveExpansionSignals), .xu3(gpuExpansionSignals));

endmodule
