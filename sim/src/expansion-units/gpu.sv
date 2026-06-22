`include "src/core.svh"

module GPU(

    input logic clk, r, byte_low,

    inout wire [15:0] bus,
    input logic [23:0] addr,

    ExpansionSignals_if.unit gpuUnitSignals

    );

    import "DPI-C" function void WriteDisplayPixel_256x256px_5bxRGB(input int pixelAddress, input int colour, input int reset);

    always @(posedge clk) begin
        
        if (r) begin
            WriteDisplayPixel_256x256px_5bxRGB(0, 0, 1);
        end else if (gpuUnitSignals.we) begin
            int pixelAddress =  { 8'b0, addr };
            int colour =        { 17'b0, bus[14:0] };
            int reset =         { 31'b0, bus[15] };

            WriteDisplayPixel_256x256px_5bxRGB(pixelAddress, colour, reset);
        end

    end

endmodule
