`include "src/core.svh"

module ProgramCounter(

    input logic clk, r,

    inout wire [15:0] bus,
    output wire [15:0] bus_alu_a,
    output wire [15:0] bus_alu_b,

    ControlSignals_if.unit controlSignals

    );

    logic [15:0] data;

    assign bus = controlSignals.bus_src[`R_PC] ? data : 16'bz;
    assign bus_alu_a = controlSignals.alu_a_sel[`R_PC] ? data : 16'bz;
    assign bus_alu_b = controlSignals.alu_b_sel[`R_PC] ? data : 16'bz;

    always @(posedge clk) begin
        
        if (r) begin
            data <= 0;
        end else if (controlSignals.bus_dest[`R_PC]) begin
            data <= bus;
        end else if (controlSignals.pc_e) begin
            data <= data + 2; // 2B Increase so 16b Word
        end

    end

endmodule
