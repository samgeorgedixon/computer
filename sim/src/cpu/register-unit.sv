`include "src/core.svh"
`include "src/cpu/reg_16bit.sv"

module RegisterUnit(

    input logic clk, r,

    inout wire [15:0] bus,
    output wire [15:0] alu_a_bus,
    output wire [15:0] alu_b_bus,

    input logic [31:0] bus_src_decoded,
    input logic [31:0] bus_dest_decoded,
    input logic [15:0] alu_a_decoded,
    input logic [15:0] alu_b_decoded,

    input logic [3:0] alu_a_sel,
    input logic [3:0] alu_b_sel,

    output logic [15:0] instr,
    output logic [15:0] mem

    );

    Reg16 r1(`REG16_FILL(`CS_R1));
    Reg16 r2(`REG16_FILL(`CS_R2));
    Reg16 r3(`REG16_FILL(`CS_R3));
    Reg16 r4(`REG16_FILL(`CS_R4));

    // Program Counter External - 5
    Reg16 mem(`REG16_FILL(`CS_MEM), mem);
    Reg16 instr(`REG16_FILL(`CS_INSTR), instr);

    Reg16 sp(`REG16_FILL(`CS_SP));
    Reg16 bp(`REG16_FILL(`CS_BP));

    Reg16 cs(`REG16_FILL(`CS_CS)); // TODO: Direct Selections
    Reg16 ds(`REG16_FILL(`CS_DS));
    Reg16 ss(`REG16_FILL(`CS_SS));
    Reg16 es(`REG16_FILL(`CS_ES));

    // Zero `Register`
    assign bus = bus_src_decoded[`CS_ZERO] ? 16'b0 : 16'bz;
    assign alu_a_bus = alu_a_decoded[`CS_ZERO] ? 16'b0 : 16'bz;
    assign alu_b_bus = alu_b_decoded[`CS_ZERO] ? 16'b0 : 16'bz;

endmodule
