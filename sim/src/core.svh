`ifndef CORE_SVH
`define CORE_SVH

// Decodes of Control Signals

// bus_src_decoded / bus_dest_decoded (0 - 32b), alu_a_decoded / alu_b_decoded (0 - 16b)
`define CS_R1 1
`define CS_R2 2
`define CS_R3 3
`define CS_R4 4

`define CS_PC 5
`define CS_MEM 6
`define CS_INSTR 7

`define CS_SP 8
`define CS_BP 9

`define CS_CS 10
`define CS_DS 11
`define CS_SS 12
`define CS_ES 13

`define CS_ZERO 14
// + Expansion Units (Memory)

// Control Signals

interface ControlSignals_if;
    logic [4:0] bus_src_raw;
    logic [4:0] bus_dest_raw;
    logic [3:0] bus_dest_special_raw;

    logic [31:0] bus_src;
    logic [31:0] bus_dest;

    logic [3:0] alu_op_sel__seg_sel;
    logic       alu__seg_pc;

    logic [3:0] alu_a_sel_raw;
    logic [3:0] alu_b_sel_raw;
    logic [15:0] alu_a_sel;
    logic [15:0] alu_b_sel;

    logic pc_e;
    logic flags_e;

    modport control (
        output bus_src_raw,
        output bus_dest_raw,
        output bus_dest_special_raw,

        output alu_op_sel__seg_sel,
        output alu__seg_pc,

        output alu_a_sel_raw,
        output alu_b_sel_raw,

        output pc_e,
        output flags_e
    );

    modport decoderIN (
        input bus_src_raw,
        input bus_dest_raw,
        input bus_dest_special_raw,

        input alu_op_sel__seg_sel,
        input alu__seg_pc,

        input alu_a_sel_raw,
        input alu_b_sel_raw,

        input pc_e,
        input flags_e
    );

    modport decoderOUT (
        output bus_src,
        output bus_dest,

        output alu_op_sel__seg_sel,
        output alu__seg_pc,

        output alu_a_sel,
        output alu_b_sel,

        output pc_e,
        output flags_e
    );

    modport unit (
        input bus_src,
        input bus_dest,

        input alu_op_sel__seg_sel,
        input alu__seg_pc,

        input alu_a_sel,
        input alu_b_sel,

        input pc_e,
        input flags_e
    );

endinterface

// Expansion Units

interface ExpansionSignals_if;
    logic we, oe;

    modport unit (
        input we, oe
    );

    modport manager (
        output we, oe
    );

endinterface

// Registers

`define REG16_FILL(i) clk, r, controlSignals.bus_dest[(i)], controlSignals.bus_src[(i)], controlSignals.alu_a_sel[(i)], controlSignals.alu_b_sel[(i)], bus, bus_alu_a, bus_alu_b

`endif
