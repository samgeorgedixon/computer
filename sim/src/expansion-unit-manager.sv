`include "src/core.svh"

module ExpansionUnitManager(

    ControlSignals_if.unit controlSignals,

    ExpansionSignals_if.manager xu1,
    ExpansionSignals_if.manager xu2,
    ExpansionSignals_if.manager xu3
    // + Add More

    );
    
    assign xu1.we = controlSignals.bus_dest[`R_XU1];
    assign xu1.oe = controlSignals.bus_src[`R_XU1];

    assign xu2.we = controlSignals.bus_dest[`R_XU2];
    assign xu2.oe = controlSignals.bus_src[`R_XU2];

    assign xu3.we = controlSignals.bus_dest[`R_XU3];
    assign xu3.oe = controlSignals.bus_src[`R_XU3];

endmodule
