module RAM_8bx24b(

    input logic clk, r,

    inout wire [15:0] bus,
    input logic [23:0] addr,

    ExpansionSignals_if.unit ramUnitSignals

    );

    logic [7:0] ram [0:(2**24) - 1];

    assign bus = ramUnitSignals.oe ? { ram[addr], ram[addr + 1] } : 16'bz; // TODO: Need to Impliment Byte BUS

    always @(posedge clk) begin

        if (r) begin
            integer i;
            for (i = 0; i < 2**24; i = i + 1) begin
                ram[i] = 8'b0;
            end
        end else if (ramUnitSignals.we) begin
            ram[addr] <= bus[15:8];
            ram[addr + 1] <= bus[7:0];
        end

    end

endmodule
