`include "src/core.svh"
`include "src/cpu/register-unit.sv"

module ProgramCounter(

    input logic clk, r,
    input logic enable,

    inout wire [15:0] bus,
    output wire [15:0] alu_a_bus,
    output wire [15:0] alu_b_bus,

    input logic [31:0] bus_src_decoded;
    input logic [31:0] bus_dest_decoded;
    input logic [15:0] alu_a_decoded;
    input logic [15:0] alu_b_decoded;

    );

    logic [15:0] pc = 0;

    assign bus = bus_oe ? pc : 16'bz;
    assign alu_a_bus = alu_a_oe ? pc : 16'bz;
    assign alu_b_bus = alu_b_oe ? pc : 16'bz;
    
    Reg16 pc(`REG16_FILL(`CS_PC));

    always @(posedge clk) begin

        if (r) begin
            pc <= 0;
        end else if (bus_we) begin
            pc <= bus;
        end else if (enable) begin
            if (pc == 'hffff) begin
                pc <= 0;
            end else begin
                pc <= pc + 2; // 2B Increase so 16b Word
            end
        end

    end

endmodule
