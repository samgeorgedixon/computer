module ALU(

    input clk,
    output [15:0] bus,
    input [3:0] ops, as, bs, input oe,
    input [15:0] r1i, r2i, r3i, r4i, spi, bpi, csi, dsi, ssi, esi, mi, pci,
    output [7:0] f

    );

    reg [16:0] res;
    wire [15:0] a, b;

    assign bus = oe ? res[15:0] : 16'bz;

    assign a = (as == 1) ? r1i : (as == 2) ? r2i : (as == 3) ? r3i : (as == 4) ? r4i : (as == 5) ? spi : (as == 6) ? bpi : (as == 7) ? csi : (as == 8) ? dsi : (as == 9) ? ssi : (as == 10) ? esi : (as == 11) ? mi : (as == 12) ? pci : 0;
    assign b = (bs == 1) ? r1i : (bs == 2) ? r2i : (bs == 3) ? r3i : (bs == 4) ? r4i : (bs == 5) ? spi : (bs == 6) ? bpi : (bs == 7) ? csi : (bs == 8) ? dsi : (bs == 9) ? ssi : (bs == 10) ? esi : (bs == 11) ? mi : (bs == 12) ? pci : 0;

    assign f[0] = res == 0 ? 1 : 0;
    assign f[1] = res[16];

    always_comb begin

        case (ops)
            0: res = {1'b0, a} + {1'b0, b};
            1: res = {1'b0, a} - {1'b0, b};
            2: res = {1'b0, a} + 1;
            3: res = {1'b0, a} - 1;
            4: res = {1'b0, a} * {1'b0, b};
            5: res = (b != 0) ? {1'b0, a} / {1'b0, b} : 17'b0;
            6: res = {1'b0, a} & {1'b0, b};
            7: res = {1'b0, a} | {1'b0, b};

            default: res = 17'b0;
        endcase

    end

endmodule
