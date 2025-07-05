module ProgramCounter(

    input clk, r,
    inout [15:0] bus,
    input we, oe, e,
    output [15:0] o

    );

    reg [15:0] pc = 0;

    assign bus = oe ? pc : 16'bz;
    assign o = pc;

    always @(posedge clk) begin

        if (we) begin
            pc <= bus;
        end

        if (r) begin
            pc <= 0;
        end else if (e) begin
            if (pc == 'hffff) begin
                pc <= 0;
            end else begin
                pc <= pc + 2;
            end
        end

    end

endmodule
