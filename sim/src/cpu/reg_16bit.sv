module Reg16(

    input logic clk, r,

    input logic bus_we, bus_oe,
    input logic alu_a_oe, alu_b_oe,

    inout wire [15:0] bus,
    output wire [15:0] alu_a_bus,
    output wire [15:0] alu_b_bus,

    output logic [15:0] data

    );

    logic [15:0] data;

    assign bus = bus_oe ? data : 16'bz;
    assign alu_a_bus = alu_a_oe ? data : 16'bz;
    assign alu_b_bus = alu_b_oe ? data : 16'bz;

    always @(posedge clk) begin
        
        if (r) begin
            data <= 0;
        end else if (bus_we) begin
            data <= bus;
        end

    end

endmodule
