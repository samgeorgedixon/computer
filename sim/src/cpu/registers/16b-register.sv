module Register16b(

    input logic clk, r,

    input logic bus_we, bus_oe,
    input logic alu_a_oe, alu_b_oe,

    inout wire [15:0] bus,
    inout wire [15:0] bus_alu_a,
    inout wire [15:0] bus_alu_b,

    output logic [15:0] data

    );

    assign bus = bus_oe ? data : 16'd0;
    assign bus_alu_a = alu_a_oe ? data : 16'd0;
    assign bus_alu_b = alu_b_oe ? data : 16'd0;

    always @(posedge clk) begin
        
        if (r) begin
            data <= 0;
        end else if (bus_we) begin
            data <= bus;
        end

    end

endmodule
