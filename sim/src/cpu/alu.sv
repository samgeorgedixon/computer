`include "src/core.svh"

module ALU(

    input r,

    output wire [15:0] bus,
    output wire [15:0] bus_alu_a,
    output wire [15:0] bus_alu_b,

    output wire [7:0] bus_flags,

    ControlSignals_if.unit controlSignals

    );

    logic [16:0] res;

    assign bus = controlSignals.alu_op_sel != 4'd0 ? res[15:0] : 16'bz;

    assign bus_flags[0] = res[15:0] == 16'd0 ? 1'd1 : 1'd0;     // Zero  Flag [0]
    assign bus_flags[1] = res[16];                              // Carry Flag [1]

    always_comb begin

        /* verilator lint_off CASEINCOMPLETE */
        case (controlSignals.alu_op_sel)
            `ALU_ADD:    res = {1'd0, bus_alu_a} + {1'd0, bus_alu_b};
            `ALU_SUB:    res = {1'd0, bus_alu_a} - {1'd0, bus_alu_b};

            `ALU_INC:    res = {1'd0, bus_alu_a} + 1;
            `ALU_INC2:   res = {1'd0, bus_alu_a} + 2;
            `ALU_DEC:    res = {1'd0, bus_alu_a} - 1;
            `ALU_DEC2:   res = {1'd0, bus_alu_a} - 2;

            `ALU_MUL:    res = {1'd0, bus_alu_a} * {1'd0, bus_alu_b};
            `ALU_DIV:    res = (bus_alu_b != 0) ? {1'd0, bus_alu_a} / {1'd0, bus_alu_b} : 17'd0;

            `ALU_AND:    res = {1'd0, bus_alu_a} & {1'd0, bus_alu_b};
            `ALU_OR:     res = {1'd0, bus_alu_a} | {1'd0, bus_alu_b};
        endcase
        /* verilator lint_on CASEINCOMPLETE */

    end

endmodule
