`include "src/core.svh"
`include "src/cpu/registers/16b-register.sv"

module RegisterUnit(

    input logic clk, r,

    inout wire [15:0] bus,
    output wire [15:0] bus_alu_a,
    output wire [15:0] bus_alu_b,

    output logic [15:0] instr_direct, memory_direct,
    output logic [15:0] cs_direct, ds_direct, ss_direct, es_direct,

    ControlSignals_if.unit controlSignals

    );

    Register16b r1(`REG16_FILL(`CS_R1), .data());
    Register16b r2(`REG16_FILL(`CS_R2), .data());
    Register16b r3(`REG16_FILL(`CS_R3), .data());
    Register16b r4(`REG16_FILL(`CS_R4), .data());

    // CS_PC
    Register16b mem(`REG16_FILL(`CS_MEM), .data(memory_direct));
    Register16b instr(`REG16_FILL(`CS_INSTR), .data(instr_direct));

    Register16b sp(`REG16_FILL(`CS_SP), .data());
    Register16b bp(`REG16_FILL(`CS_BP), .data());

    Register16b cs(`REG16_FILL(`CS_CS), .data(cs_direct));
    Register16b ds(`REG16_FILL(`CS_DS), .data(ds_direct));
    Register16b ss(`REG16_FILL(`CS_SS), .data(ss_direct));
    Register16b es(`REG16_FILL(`CS_ES), .data(es_direct));

    // Zero `Register`
    assign bus = controlSignals.bus_src[`CS_ZERO] ? 16'b0 : 16'bz;
    assign bus_alu_a = controlSignals.alu_a_sel[`CS_ZERO] ? 16'b0 : 16'bz;
    assign bus_alu_b = controlSignals.alu_b_sel[`CS_ZERO] ? 16'b0 : 16'bz;

endmodule
