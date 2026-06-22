`include "src/core.svh"
`include "src/cpu/registers/16b-register.sv"

module RegisterUnit(

    input logic clk, r,

    inout wire [15:0] bus,
    output wire [15:0] bus_alu_a,
    output wire [15:0] bus_alu_b,

    output logic [15:0] ir_direct, ar_direct,
    output logic [15:0] cs_direct, ds_direct, ss_direct, es_direct,

    ControlSignals_if.unit controlSignals

    );

    Register16b r1(`REG16_FILL(`R_R1), .data());
    Register16b r2(`REG16_FILL(`R_R2), .data());
    Register16b r3(`REG16_FILL(`R_R3), .data());
    Register16b r4(`REG16_FILL(`R_R4), .data());

    Register16b sp(`REG16_FILL(`R_SP), .data());
    Register16b bp(`REG16_FILL(`R_BP), .data());

    Register16b cs(`REG16_FILL(`R_CS), .data(cs_direct));
    Register16b ds(`REG16_FILL(`R_DS), .data(ds_direct));
    Register16b ss(`REG16_FILL(`R_SS), .data(ss_direct));
    Register16b es(`REG16_FILL(`R_ES), .data(es_direct));

    // R_IP
    Register16b ir(`REG16_FILL(`R_IR), .data(ir_direct));
    Register16b ar(`REG16_FILL(`R_AR), .data(ar_direct));

    // Zero `Register`
    assign bus = controlSignals.bus_src[`R_ZERO] ? 16'b0 : 16'bz;
    assign bus_alu_a = controlSignals.alu_a_sel[`R_ZERO] ? 16'b0 : 16'bz;
    assign bus_alu_b = controlSignals.alu_b_sel[`R_ZERO] ? 16'b0 : 16'bz;

endmodule
