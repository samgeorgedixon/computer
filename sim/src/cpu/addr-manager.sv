`include "src/core.svh"

module AddrManager(

    output logic [23:0] addr,

    AddressRegisters_if.manager addrRegisters,

    ControlSignals_if.unit controlSignals

    );

    logic [23:0] addr_base_full;
    logic [23:0] addr_segment;

    assign addr_base_full = { 8'b0, addrRegisters.ar };

    always_comb begin
        addr_segment = 24'b0;

        if      (controlSignals.seg_sel[0]) addr_segment[23:8] = addrRegisters.cs;
        else if (controlSignals.seg_sel[1]) addr_segment[23:8] = addrRegisters.ds;
        else if (controlSignals.seg_sel[2]) addr_segment[23:8] = addrRegisters.ss;
        else if (controlSignals.seg_sel[3]) addr_segment[23:8] = addrRegisters.es;
    end

    assign addr = addr_base_full + addr_segment;

endmodule
