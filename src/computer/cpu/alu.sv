module ALU(

    input clk,
    input [15:0] a, b,
    input [2:0] sel, input oe,
    output [15:0] o,
    output [7:0] f

    );

    reg [16:0] res;

    assign o = oe ? res[15:0] : 16'bz;

    assign f[0] = res == 0 ? 1 : 0;
    assign f[1] = res[16];

    always_comb begin
        case (sel)
            3'b000: res = {1'b0, a} + {1'b0, b};
            3'b001: res = {1'b0, a} - {1'b0, b};
            3'b010: res = {1'b0, a} + 1;
            3'b011: res = {1'b0, a} - 1;
            3'b100: res = {1'b0, a} * {1'b0, b};
            3'b101: res = (b != 0) ? {1'b0, a} / {1'b0, b} : 17'b0;
            3'b110: res = {1'b0, a} & {1'b0, b};
            3'b111: res = {1'b0, a} | {1'b0, b};
            default: res = 17'b0;
        endcase
    end

endmodule
