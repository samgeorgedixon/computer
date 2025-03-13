module ProgramCounter16bit(

    input we, oe, e,
    input clk, r,
    input [15:0] a, output [15:0] o

    );

    reg [15:0] pc = 0;

    assign o = oe ? pc : o;

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
