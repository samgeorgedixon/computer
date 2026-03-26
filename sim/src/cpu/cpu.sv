`include "src/core.svh"
`include "src/cpu/control-unit.sv"
`include "src/cpu/register-unit.sv"
`include "src/cpu/program-counter.sv"

module CPU(
    
    input logic clk, r,
    inout wire [15:0] bus,

    input [23:0] addro

    );

    wire [15:0] alu_a_bus,
    wire [15:0] alu_b_bus,

    // Direct Registers
    logic [15:0] instr;
    logic [15:0] mem;

    logic [7:0] flags;

    // Control Signals
    logic [4:0] bus_src,
    logic [4:0] bus_dest,
    logic [3:0] bus_dest_special,
    
    logic [31:0] bus_src_decoded;
    logic [31:0] bus_dest_decoded;
    logic [15:0] alu_a_decoded;
    logic [15:0] alu_b_decoded;

    logic [3:0] alu_op_sel__seg_sel,
    logic       alu__seg_pc,

    logic [3:0] alu_a_sel,
    logic [3:0] alu_b_sel,

    logic pc_e,
    logic flags_e

    // Units
    ControlUnit cu(clk, r, instr, flags, bus_src, bus_dest, bus_dest_special, alu_op_sel__seg_sel, alu__seg_pc, alu_a_sel, alu_b_sel, pc_e, flags_e); // Finish Instruction Set

    Decoder_CU decoder(clk, r, bus_src, bus_dest, bus_dest_special, bus_src_decoded, bus_dest_decoded, alu_a_decoded, alu_b_decoded);

    RegisterUnit ru(clk, r, bus, alu_a_bus, alu_b_bus, bus_src_decoded, bus_dest_decoded, alu_a_decoded, alu_b_decoded, alu_a_sel, alu_b_sel, instr, mem);

    ProgramCounter pc(clk, r, pc_e, bus, alu_a_bus, alu_b_bus, bus_src_decoded, bus_dest_decoded, alu_a_decoded, alu_b_decoded);

    // + ALU + Extension Units

endmodule
