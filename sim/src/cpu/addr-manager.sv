`include "src/core.svh"

module AddrManager(

    output logic [23:0] addr,

    input logic [15:0] addr_base,
    input logic [15:0] cs_direct, ds_direct, ss_direct, es_direct,

    ControlSignals_if.unit controlSignals

    );

    logic [23:0] addr_base_full;
    logic [23:0] addr_segment;

    assign addr_base_full = { 8'b0, addr_base };

    always_comb begin
        addr_segment = 24'b0;

        if      (controlSignals.seg_sel[0]) addr_segment[23:8] = cs_direct;
        else if (controlSignals.seg_sel[1]) addr_segment[23:8] = ds_direct;
        else if (controlSignals.seg_sel[2]) addr_segment[23:8] = ss_direct;
        else if (controlSignals.seg_sel[3]) addr_segment[23:8] = es_direct;
    end

    assign addr = addr_base_full + addr_segment;

endmodule
