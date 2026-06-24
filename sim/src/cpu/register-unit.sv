`include "src/core.svh"
`include "src/cpu/registers/16b-register.sv"

`define REG16_FILL(i) \
    .clk(clk), .r(r), \
    .bus_we(controlSignals.bus_dest[(i)]), .bus_oe(controlSignals.bus_src[(i)]), .alu_a_oe(controlSignals.alu_a_sel[(i)]), .alu_b_oe(controlSignals.alu_b_sel[(i)]), \
    .bus(bus), .bus_alu_a(bus_alu_a), .bus_alu_b(bus_alu_b)

module RegisterUnit(

    input logic clk, r,

    inout wire [15:0] bus,
    output wire [15:0] bus_alu_a,
    output wire [15:0] bus_alu_b,

    output logic [15:0] ir_direct, 
    AddressRegisters_if.registers addrRegisters,

    ControlSignals_if.unit controlSignals

    );

    // Zero `Register
    assign bus = 16'd0; //controlSignals.bus_src[`R_ZERO] ? 16'd0 : 16'd0;
    assign bus_alu_a = 16'd0; //controlSignals.alu_a_sel[`R_ZERO] ? 16'b0 : 16'd0;
    assign bus_alu_b = 16'd0; //controlSignals.alu_b_sel[`R_ZERO] ? 16'b0 : 16'd0;

    Register16b r1(`REG16_FILL(`R_R1), .data());
    Register16b r2(`REG16_FILL(`R_R2), .data());
    Register16b r3(`REG16_FILL(`R_R3), .data());
    Register16b r4(`REG16_FILL(`R_R4), .data());

    Register16b sp(`REG16_FILL(`R_SP), .data());
    Register16b bp(`REG16_FILL(`R_BP), .data());

    Register16b cs(`REG16_FILL(`R_CS), .data(addrRegisters.cs));
    Register16b ds(`REG16_FILL(`R_DS), .data(addrRegisters.ds));
    Register16b ss(`REG16_FILL(`R_SS), .data(addrRegisters.ss));
    Register16b es(`REG16_FILL(`R_ES), .data(addrRegisters.es));

    // R_IP
    Register16b ir(`REG16_FILL(`R_IR), .data(ir_direct));
    Register16b ar(`REG16_FILL(`R_AR), .data(addrRegisters.ar));

endmodule
