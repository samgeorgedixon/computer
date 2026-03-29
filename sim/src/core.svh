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

`define CS_EXP1 16 // RAM
`define CS_EXP2 17 // Drive
`define CS_EXP3 18
`define CS_EXP4 19

// bus_dest_special_raw
`define SPECIAL_MEM_SP 1
 
// Control Signals

interface ControlSignals_if;
    logic [4:0]     bus_src_raw;
    logic [4:0]     bus_dest_raw;
    logic [3:0]     bus_dest_special_raw; // TODO: Decrease to 1b / 2b

    logic [31:0]    bus_src;
    logic [31:0]    bus_dest;

    // No Memory use while using ALU
    logic [3:0]     seg_sel__alu_op_sel_raw;    // __ or - [0]-[3]: Segment Select, 0-15: ALU OP Select
    logic           alu_e_raw;                  // 0: Segment On / ALU Off, 1: Segment On / ALU On
    logic [3:0]     seg_sel;                    // 0-3: Segment Select (No Decoded Needed)
    logic [3:0]     alu_op_sel;                 // Not Decoded: For ALU CPLD

    logic [3:0]     alu_a_sel_raw;
    logic [3:0]     alu_b_sel_raw;
    logic [15:0]    alu_a_sel;
    logic [15:0]    alu_b_sel;

    logic           pc_e;
    logic           flags_e;

    logic [1:0]     operand_0_raw; // (00: Off, 01: seg_sel, 10: bus_dest, 11: bus_src)
    logic [1:0]     operand_1_raw; // (00: Off, 01: bus_dest, 10: bus_src, 01&alu_e_raw: alu-a-sel)
    logic           operand_2_raw; // (0: Off, 1&alu_e_raw: alu-b-sel, 1: bus_src)

    modport control (
        output bus_src_raw,
        output bus_dest_raw,
        output bus_dest_special_raw,

        output seg_sel__alu_op_sel_raw,
        output alu_e_raw,

        output alu_a_sel_raw,
        output alu_b_sel_raw,

        output pc_e,
        output flags_e,

        output operand_0_raw,
        output operand_1_raw,
        output operand_2_raw
    );

    modport decoderIN (
        input bus_src_raw,
        input bus_dest_raw,
        input bus_dest_special_raw,

        input seg_sel__alu_op_sel_raw,
        input alu_e_raw,

        input alu_a_sel_raw,
        input alu_b_sel_raw,

        input pc_e,
        input flags_e,

        input operand_0_raw,
        input operand_1_raw,
        input operand_2_raw
    );

    modport decoderOUT (
        output bus_src,
        output bus_dest,

        output seg_sel,
        output alu_op_sel,

        output alu_a_sel,
        output alu_b_sel,

        output pc_e,
        output flags_e
    );

    modport unit (
        input bus_src,
        input bus_dest,

        input seg_sel,
        input alu_op_sel,

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

`define REG16_FILL(i) \
    .clk(clk), .r(r), \
    .bus_we(controlSignals.bus_dest[(i)]), .bus_oe(controlSignals.bus_src[(i)]), .alu_a_oe(controlSignals.alu_a_sel[(i)]), .alu_b_oe(controlSignals.alu_b_sel[(i)]), \
    .bus(bus), .bus_alu_a(bus_alu_a), .bus_alu_b(bus_alu_b)

`endif
