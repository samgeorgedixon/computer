module RAM8B24A(

    input clk, r,

    input we, oe,

    input [23:0] addr,
    inout [15:0] data

    );



    always @(posedge clk) begin

        if (r) begin
            integer i;
            for (i = 0; i < 2**24; i = i + 1) begin
                memory[i] = 8'b0;
            end
        end else if (we) begin
            memory[addr + 1] <= data[7:0];
            memory[addr] <= data[15:8];
        end

    end

endmodule
