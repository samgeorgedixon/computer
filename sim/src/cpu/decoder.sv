`include "src/core.svh"

module Decoder(

    ControlSignals_if.decoderIN controlSignalsRaw, // TODO: Impliment Specials
    ControlSignals_if.decoderOUT controlSignals

    );

    assign controlSignals.bus_src = 32'b1 << controlSignalsRaw.bus_src_raw;
    assign controlSignals.bus_dest = 32'b1 << controlSignalsRaw.bus_dest_raw;

    assign controlSignals.seg_sel__alu_op_sel = 16'b1 << controlSignalsRaw.seg_sel__alu_op_sel_raw;
    assign controlSignals.seg__alu = controlSignalsRaw.seg__alu;

    assign controlSignals.alu_a_sel = 16'b1 << controlSignalsRaw.alu_a_sel_raw;
    assign controlSignals.alu_b_sel = 16'b1 << controlSignalsRaw.alu_b_sel_raw;

    assign controlSignals.pc_e = controlSignalsRaw.pc_e;
    assign controlSignals.flags_e = controlSignalsRaw.flags_e;

endmodule
