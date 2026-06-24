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
    output logic [23:0] addr,

    output logic int_e, byte_low, 

    ExpansionUnitSignals_if xu[16]

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

    assign int_e = controlSignals.int_e;
    assign byte_low = controlSignals.byte_low;
    
    logic [15:0] xu_irq;
    logic irq_state;

    // Units

    ControlUnit controlUnit(clk, r, ir_direct, flags_direct, irq_state, controlSignalsRaw);

    AddressRegisters_if addrRegisters();
    RegisterUnit registerUnit(clk, r, bus, bus_alu_a, bus_alu_b, ir_direct, addrRegisters, xu_irq, controlSignals);

    ALU alu(r, bus, bus_alu_a, bus_alu_b, bus_flags, controlSignals);

    InstructionPointerUnit programCounter(clk, r, bus, bus_alu_a, bus_alu_b, controlSignals);
    
    AddrManager addrManager(addr, addrRegisters, controlSignals);
    ExpansionUnitManager xuManager(controlSignals, xu, xu_irq, irq_state);

    Decoder decoder(ir_direct, controlSignalsRaw, controlSignals);
    Flags flags(clk, r, bus_flags, flags_direct, controlSignals);


endmodule
