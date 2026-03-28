`include "src/core.svh"

module ExpansionUnitManager(

    ControlSignals_if.unit controlSignals,

    ExpansionSignals_if.manager expUnit1,
    ExpansionSignals_if.manager expUnit2,
    ExpansionSignals_if.manager expUnit3
    // + Add More

    );
    
    assign expUnit1.we = controlSignals.bus_dest[`CS_EXP1];
    assign expUnit1.oe = controlSignals.bus_src[`CS_EXP1];

    assign expUnit2.we = controlSignals.bus_dest[`CS_EXP2];
    assign expUnit2.oe = controlSignals.bus_src[`CS_EXP2];

    assign expUnit3.we = controlSignals.bus_dest[`CS_EXP3];
    assign expUnit3.oe = controlSignals.bus_src[`CS_EXP3];

endmodule
