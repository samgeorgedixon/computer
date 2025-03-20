module ProgramCounter(

    input clk, r,
    input [15:0] a,
    input we, oe, e,
    output [15:0] o

    );

    reg [15:0] pc = 0;

    assign o = oe ? pc : 16'bz;

    always @(posedge clk) begin

        if (we) begin
            pc <= a;
        end

        if (r) begin
            pc <= 0;
        end else if (e) begin
            if (pc == 'hffff) begin
                pc <= 0;
            end else begin
                pc <= pc + 1;
            end
        end

    end

endmodule
