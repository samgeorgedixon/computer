module RAM16B24A(

    input clk, r,

    input we, oe,

    input [23:0] addr,
    inout [15:0] data

    );

    reg [15:0] memory [0:(2**24)-1];

    assign data = oe ? memory[addr] : 16'bz;

    always @(posedge clk) begin

        if (r) begin
            integer i;
            for (i = 0; i < 2**24; i = i + 1) begin
                memory[i] = 16'b1;
            end
        end else if (we) begin
            memory[addr] <= data;
        end

    end

endmodule
