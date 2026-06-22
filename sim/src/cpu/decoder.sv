`include "src/core.svh"

module Decoder(

    input logic [15:0] instr,

    ControlSignals_if.decoderIN controlSignalsRaw,
    ControlSignals_if.decoderOUT controlSignals

    );

    logic [1:0] operand_0;
    logic [3:0] operand_1;
    logic [3:0] operand_2;

    assign operand_0 = instr[9:8];
    assign operand_1 = instr[7:4];
    assign operand_2 = instr[3:0];

    always_comb begin
        controlSignals.bus_src = 32'b1 << controlSignalsRaw.bus_src_raw;
        controlSignals.bus_dest = 32'b1 << controlSignalsRaw.bus_dest_raw;
        
        controlSignals.alu_op_sel = controlSignalsRaw.alu_e_raw ? controlSignalsRaw.seg_sel_or_alu_op_sel : 4'b0;
        controlSignals.alu_a_sel = 16'b1 << controlSignalsRaw.alu_a_sel_raw;
        controlSignals.alu_b_sel = 16'b1 << controlSignalsRaw.alu_b_sel_raw;

        /* verilator lint_off CASEINCOMPLETE */
        unique case (controlSignalsRaw.operand_0)
            `OP0_BUS_DEST: controlSignals.bus_dest[`R_R4:`R_R1] |= 4'b1 << operand_0;
            `OP0_BUS_SRC: controlSignals.bus_src[`R_R4:`R_R1] |= 4'b1 << operand_0;
            `OP0_SEG_SEL: controlSignals.seg_sel = controlSignalsRaw.seg_sel_or_alu_op_sel;
        endcase
        unique case (controlSignalsRaw.operand_1) // TODO: Check SEL_ES in regard to ip_e
            `OP1_BUS_DEST: begin
                if (controlSignalsRaw.seg_sel_or_alu_op_sel == `SEL_ES)   controlSignals.bus_dest[31:16] |= 16'b1 << operand_1;
                else                                                        controlSignals.bus_dest[15:0] |= 16'b1 << operand_1;
            end
            `OP1_BUS_SRC_OR_ALU_A_SEL: begin
                if (controlSignalsRaw.seg_sel_or_alu_op_sel == `SEL_ES)   controlSignals.bus_src[31:16] |= 16'b1 << operand_1;
                else if (controlSignalsRaw.alu_e_raw)                       controlSignals.alu_a_sel |= 16'b1 << operand_1;
                else                                                        controlSignals.bus_src[15:0] |= 16'b1 << operand_1;
            end
            `OP1_BUS_DEST_AND_ALU_A_SEL: begin
                controlSignals.bus_dest[15:0] |= 16'b1 << operand_1;
                controlSignals.alu_a_sel |= 16'b1 << operand_1;
            end
        endcase
        unique case (controlSignalsRaw.operand_2)
            `OP2_BUS_DEST: controlSignals.bus_dest[15:0] |= 16'b1 << operand_2;
            `OP2_BUS_SRC_OR_ALU_B_SEL: begin
                if (controlSignalsRaw.alu_e_raw)    controlSignals.alu_b_sel |= 16'b1 << operand_2;
                else                                controlSignals.bus_src[15:0] |= 16'b1 << operand_2;
            end
            `OP2_ALU_A_SEL: controlSignals.alu_a_sel |= 16'b1 << operand_2;
        endcase
        /* verilator lint_on CASEINCOMPLETE */
        
        if (controlSignalsRaw.bus_dest_ar_and_sp) begin
            controlSignals.bus_dest[`R_AR] |= 1'd1;
            controlSignals.bus_dest[`R_SP] |= 1'd1;
        end


        controlSignals.ip_e = controlSignalsRaw.ip_e;
        controlSignals.flags_e = controlSignalsRaw.flags_e;
        controlSignals.byte_low = controlSignalsRaw.byte_low;
    end

endmodule
