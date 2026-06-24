`include "src/core.svh"

module ExpansionUnitManager(

    ControlSignals_if.unit controlSignals,

    ExpansionUnitSignals_if.manager xu[16],

    output logic [15:0] xu_irq,
    output logic irq_state

    );

    assign irq_state = (xu_irq != 16'd0);

    for (genvar i = 0; i < 16; i++) begin : g_xu
        assign xu[i].we = controlSignals.bus_dest[`R_XU1 + i];
        assign xu[i].oe = controlSignals.bus_src[`R_XU1 + i];

        assign xu_irq[i] = xu[i].irq;
    end
    
endmodule
