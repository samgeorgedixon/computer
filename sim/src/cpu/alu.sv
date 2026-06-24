`include "src/core.svh"

module ALU(

    input r,

    output wire [15:0] bus,
    input wire [15:0] bus_alu_a,
    input wire [15:0] bus_alu_b,

    output logic [7:0] bus_flags,

    ControlSignals_if.unit controlSignals

    );

    logic [15:0] res;
    logic [16:0] res_ext;

    assign bus = controlSignals.alu_op_sel != 4'd0 ? res[15:0] : 16'b0;

    always_comb begin

        res = 16'd0;
        res_ext = 17'd0;

        bus_flags[`F_CARRY] = 1'd0;
        bus_flags[`F_OVERFLOW] = 1'd0;

        /* verilator lint_off CASEINCOMPLETE */
        unique case (controlSignals.alu_op_sel)
            4'd0: res = 16'd0;
            `ALU_ADD: begin
                res_ext = {1'd0, bus_alu_a} + {1'd0, bus_alu_b};
                res = res_ext[15:0];

                bus_flags[`F_CARRY] = res_ext[16];
                bus_flags[`F_OVERFLOW] = (~(bus_alu_a[15] ^ bus_alu_b[15])) & (res[15] ^ bus_alu_a[15]);
            end
            `ALU_SUB: begin
                res_ext = {1'd0, bus_alu_a} - {1'd0, bus_alu_b};
                res = res_ext[15:0];

                bus_flags[`F_CARRY] = res_ext[16]; // 1 -> no borrow, 0 -> borrow
                bus_flags[`F_OVERFLOW] = (bus_alu_a[15] ^ bus_alu_b[15]) & (res[15] ^ bus_alu_a[15]);
            end

            `ALU_INC: begin
                res_ext = {1'd0, bus_alu_a} + 17'd1;
                res = res_ext[15:0];

                bus_flags[`F_CARRY] = res_ext[16];
                bus_flags[`F_OVERFLOW] = (~(bus_alu_a[15] ^ 1'd0)) & (res[15] ^ bus_alu_a[15]);
            end
            `ALU_INC2: begin
                res_ext = {1'd0, bus_alu_a} + 17'd2;
                res = res_ext[15:0];

                bus_flags[`F_CARRY] = res_ext[16];
                bus_flags[`F_OVERFLOW] = (~(bus_alu_a[15] ^ 1'd0)) & (res[15] ^ bus_alu_a[15]);
            end
            `ALU_DEC: begin
                res_ext = {1'd0, bus_alu_a} - 17'd1;
                res = res_ext[15:0];

                bus_flags[`F_CARRY] = res_ext[16]; // 1 -> no borrow, 0 -> borrow
                bus_flags[`F_OVERFLOW] = (bus_alu_a[15] ^ 1'd0) & (res[15] ^ bus_alu_a[15]);
            end
            `ALU_DEC2: begin
                res_ext = {1'd0, bus_alu_a} - 17'd2;
                res = res_ext[15:0];

                bus_flags[`F_CARRY] = res_ext[16]; // 1 -> no borrow, 0 -> borrow
                bus_flags[`F_OVERFLOW] = (bus_alu_a[15] ^ 1'd0) & (res[15] ^ bus_alu_a[15]);
            end

            `ALU_NOT:    res = ~bus_alu_a;
            `ALU_AND:    res = bus_alu_a & bus_alu_b;
            `ALU_OR:     res = bus_alu_a | bus_alu_b;
            `ALU_XOR:    res = bus_alu_a ^ bus_alu_b;

            `ALU_SLL: begin
                res_ext = {1'd0, bus_alu_a} << bus_alu_b[3:0];
                res = res_ext[15:0];
                bus_flags[`F_CARRY] = res_ext[16];
            end
            `ALU_SRL: begin
                res_ext = {bus_alu_a, 1'd0} >> bus_alu_b[3:0];
                res = res_ext[16:1];

                bus_flags[`F_CARRY] = res_ext[0];
            end
            `ALU_SRA: begin
                res_ext = $signed({bus_alu_a, 1'd0}) >>> bus_alu_b[3:0];
                res = res_ext[16:1];
                
                bus_flags[`F_CARRY] = res_ext[0];
            end
            `ALU_NEG:    begin
                res = ~bus_alu_a + 16'd1;

                bus_flags[`F_CARRY] = (bus_alu_a == 16'd0);
                bus_flags[`F_OVERFLOW] = (bus_alu_a == 16'h8000);
            end
        endcase
        /* verilator lint_on CASEINCOMPLETE */

        bus_flags[`F_ZERO] = (res == 16'd0);
        bus_flags[`F_SIGN] = res[15];

    end

endmodule
