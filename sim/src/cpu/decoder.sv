`include "src/core.svh"

module Decoder(

    input logic [15:0] instr,

    ControlSignals_if.decoderIN controlSignalsRaw,
    ControlSignals_if.decoderOUT controlSignals

    ); // TODO: Document Decodes

    logic [1:0] operand_0;
    logic [3:0] operand_1;
    logic [3:0] operand_2;

    assign operand_0 = instr[9:8];
    assign operand_1 = instr[7:4];
    assign operand_2 = instr[3:0];

    always_comb begin
        controlSignals.bus_src      = 0;
        controlSignals.bus_src      |= 32'b1 << controlSignalsRaw.bus_src_raw;
        controlSignals.bus_src[3:0] |= controlSignalsRaw.operand_0_raw == 2'd3 ? 4'b1 << operand_0 : 4'b0;
        controlSignals.bus_src[15:0] |= controlSignalsRaw.operand_1_raw == 2'd2 && !controlSignalsRaw.seg_sel__alu_op_sel_raw[3] ? 16'b1 << operand_1 : 16'b0; // ES
        controlSignals.bus_src[31:16] |= controlSignalsRaw.operand_1_raw == 2'd2 && controlSignalsRaw.seg_sel__alu_op_sel_raw[3] ? 16'b1 << operand_1 : 16'b0;
        controlSignals.bus_src[15:0] |= controlSignalsRaw.operand_2_raw == 1'd1 && !controlSignalsRaw.alu_e_raw ? 16'b1 << operand_2 : 16'b0;

        controlSignals.bus_dest      = 0;
        controlSignals.bus_dest      |= 32'b1 << controlSignalsRaw.bus_dest_raw;
        controlSignals.bus_dest[3:0] |= controlSignalsRaw.operand_0_raw == 2'd2 ? 4'b1 << operand_0 : 4'b0;
        controlSignals.bus_dest[15:0] |= controlSignalsRaw.operand_1_raw == 2'd1 && !controlSignalsRaw.alu_e_raw && !controlSignalsRaw.seg_sel__alu_op_sel_raw[3] ? 16'b1 << operand_1 : 16'b0;
        controlSignals.bus_dest[31:16] |= controlSignalsRaw.operand_1_raw == 2'd1 && !controlSignalsRaw.alu_e_raw && controlSignalsRaw.seg_sel__alu_op_sel_raw[3] ? 16'b1 << operand_1 : 16'b0;
        controlSignals.bus_dest[`CS_MEM] |= controlSignalsRaw.bus_dest_special_raw == 4'd`SPECIAL_MEM_SP ? 1'd1 : 1'd0;
        controlSignals.bus_dest[`CS_SP] |= controlSignalsRaw.bus_dest_special_raw == 4'd`SPECIAL_MEM_SP ? 1'd1 : 1'd0;

        controlSignals.seg_sel      = 0;
        controlSignals.seg_sel      |= controlSignalsRaw.seg_sel__alu_op_sel_raw;
        controlSignals.seg_sel      |= controlSignalsRaw.operand_0_raw == 2'd1 ? 4'b1 << operand_0 : 4'b0;

        controlSignals.alu_op_sel = controlSignalsRaw.alu_e_raw ? controlSignalsRaw.seg_sel__alu_op_sel_raw : 4'b0;

        controlSignals.alu_a_sel = 0;
        controlSignals.alu_a_sel |= 16'b1 << controlSignalsRaw.alu_a_sel_raw;
        controlSignals.alu_a_sel |= controlSignalsRaw.operand_1_raw == 2'd1 && controlSignalsRaw.alu_e_raw ? 16'b1 << operand_1 : 16'b0;

        controlSignals.alu_b_sel = 0;
        controlSignals.alu_b_sel |= 16'b1 << controlSignalsRaw.alu_b_sel_raw;
        controlSignals.alu_b_sel |= controlSignalsRaw.operand_2_raw == 1'd1 && controlSignalsRaw.alu_e_raw ? 16'b1 << operand_2 : 16'b0;

        controlSignals.pc_e = controlSignalsRaw.pc_e;
        controlSignals.flags_e = controlSignalsRaw.flags_e;
    end

endmodule
