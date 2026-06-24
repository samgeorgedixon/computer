`ifndef CORE_SVH
`define CORE_SVH

`include "src/indexes.svh"

// Control Signals

interface ControlSignals_if;
    logic [4:0]     bus_src_raw;
    logic [4:0]     bus_dest_raw;
    logic           bus_dest_ar_and_sp;

    logic [31:0]    bus_src;
    logic [31:0]    bus_dest;

    // No Memory use while using ALU
    logic [3:0]     seg_sel_or_alu_op_sel;    // __ or - [0]-[3]: Segment Select, 0-15: ALU OP Select
    logic           alu_e_raw;                  // 0: Segment On / ALU Off, 1: Segment On / ALU On
    logic [3:0]     seg_sel;                    // 0-3: Segment Select (No Decoded Needed)
    logic [3:0]     alu_op_sel;                 // Not Decoded: For ALU CPLD

    logic [3:0]     alu_a_sel_raw;
    logic [3:0]     alu_b_sel_raw;
    logic [15:0]    alu_a_sel;
    logic [15:0]    alu_b_sel;

    logic           ip_e;
    logic           flags_e;
    logic           int_e;
    logic           byte_low;

    logic [1:0]     operand_0;
    logic [1:0]     operand_1;
    logic [1:0]     operand_2;

    modport control (
        output bus_src_raw,
        output bus_dest_raw,
        output bus_dest_ar_and_sp,

        output seg_sel_or_alu_op_sel,
        output alu_e_raw,

        output alu_a_sel_raw,
        output alu_b_sel_raw,

        output ip_e,
        output flags_e,
        output int_e,
        output byte_low,

        output operand_0,
        output operand_1,
        output operand_2
    );

    modport decoderIN (
        input bus_src_raw,
        input bus_dest_raw,
        input bus_dest_ar_and_sp,

        input seg_sel_or_alu_op_sel,
        input alu_e_raw,

        input alu_a_sel_raw,
        input alu_b_sel_raw,

        input ip_e,
        input flags_e,
        input int_e,
        input byte_low,

        input operand_0,
        input operand_1,
        input operand_2
    );

    modport decoderOUT (
        output bus_src,
        output bus_dest,

        output seg_sel,
        output alu_op_sel,

        output alu_a_sel,
        output alu_b_sel,

        output ip_e,
        output flags_e,
        output int_e,
        output byte_low
    );

    modport unit (
        input bus_src,
        input bus_dest,

        input seg_sel,
        input alu_op_sel,

        input alu_a_sel,
        input alu_b_sel,

        input ip_e,
        input flags_e,
        input int_e,
        input byte_low
    );

endinterface

// Expansion Units

interface ExpansionUnitCommon_if(

    input logic clk, r,
    input logic [23:0] addr,
    input logic int_e, byte_low

);

    modport unit (
        input clk, r,
        input addr,
        input int_e, byte_low
    );

endinterface

interface ExpansionUnitSignals_if;
    logic we, oe;
    logic irq;

    modport unit (
        input we, oe,
        output irq
    );

    modport manager (
        output we, oe,
        input irq
    );

endinterface

interface AddressRegisters_if;
    logic [15:0] ar, cs, ds, ss, es;

    modport registers (
        output ar, cs, ds, ss, es
    );

    modport manager (
        input ar, cs, ds, ss, es
    );

endinterface

`endif
