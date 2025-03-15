module ALU(

    input clk,
    input [15:0] a, b,
    input [2:0] sel, input oe,
    output [15:0] o,
    output [7:0] f

    );

    reg [16:0] res = 0;

    assign o = oe ? res[15:0] : 16'bz;

    assign f[0] = res == 0 ? 1 : 0;
    assign f[1] = res[16];

    always @(posedge clk) begin

        res <= 17'b0;

        if (oe) begin

            if          (sel == 'b000) begin
                res <= {1'b0, a} + {1'b0, b};
            end else if (sel == 'b001) begin
                res <= {1'b0, a} - {1'b0, b};
            end else if (sel == 'b010) begin
                res <= {1'b0, a} + 1;
            end else if (sel == 'b011) begin
                res <= {1'b0, a} - 1;
            end else if (sel == 'b100) begin
                res <= {1'b0, a} * {1'b0, b};
            end else if (sel == 'b101) begin
                res <= {1'b0, a} / {1'b0, b};
            end else if (sel == 'b110) begin
                res <= {1'b0, a} & {1'b0, b};
            end else if (sel == 'b111) begin
                res <= {1'b0, a} | {1'b0, b};
            end

        end

    end

endmodule
