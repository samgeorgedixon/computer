`include "src/core.svh"

module Decoder_CU(

    input logic clk, r,

    input logic [4:0] bus_src,
    input logic [4:0] bus_dest,
    input logic [3:0] bus_dest_special, // TODO: Impliment

    output logic [31:0] bus_src_decoded,
    output logic [31:0] bus_dest_decoded,
    output logic [15:0] alu_a_decoded,
    output logic [15:0] alu_b_decoded

    );

    logic [31:0] bus_src_decoded;
    assign bus_src_decoded = 32'b1 << bus_src;
    logic [31:0] bus_dest_decoded;
    assign bus_dest_decoded = 32'b1 << bus_dest;

    logic [15:0] alu_a_decoded;
    assign alu_a_decoded = 16'b1 << alu_a_sel;
    logic [15:0] alu_b_decoded;
    assign alu_b_decoded = 16'b1 << alu_b_sel;

endmodule
