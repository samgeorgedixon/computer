`include "src/core.svh"

module Decoder(

    ControlSignals_if.decoderIN controlSignalsRaw, // TODO: Impliment Specials
    ControlSignals_if.decoderOUT controlSignals

    );

    assign controlSignals.bus_src = 32'b1 << controlSignalsRaw.bus_src_raw;
    assign controlSignals.bus_dest = 32'b1 << controlSignalsRaw.bus_dest_raw;

    assign controlSignals.alu_op_sel__seg_sel = controlSignalsRaw.alu_op_sel__seg_sel;
    assign controlSignals.alu__seg_pc = controlSignalsRaw.alu__seg_pc;

    assign controlSignals.alu_a_sel = 16'b1 << controlSignalsRaw.alu_a_sel_raw;
    assign controlSignals.alu_b_sel = 16'b1 << controlSignalsRaw.alu_b_sel_raw;

    assign controlSignals.pc_e = controlSignalsRaw.pc_e;
    assign controlSignals.flags_e = controlSignalsRaw.flags_e;

endmodule
