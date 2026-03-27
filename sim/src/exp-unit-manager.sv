`include "src/core.svh"

module ExpansionUnitManager(

    input logic clk, r,

    input logic [31:0] bus_src_decoded,
    input logic [31:0] bus_dest_decoded,

    ExpansionSignals_if.manager expUnit1,
    ExpansionSignals_if.manager expUnit2,
    ExpansionSignals_if.manager expUnit3,
    ExpansionSignals_if.manager expUnit4,
    ExpansionSignals_if.manager expUnit5
    // + Add More

    );
    


endmodule
