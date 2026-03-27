`include "src/core.svh"

module Flags(

    input logic clk, r,

    input wire [7:0] bus_flags,
    output logic [7:0] data,

    ControlSignals_if.unit controlSignals

    );

    always @(posedge clk) begin
        
        if (r) begin
            data <= 0;
        end else if (controlSignals.flags_e) begin
            data <= bus_flags[7:0];
        end

    end

endmodule
