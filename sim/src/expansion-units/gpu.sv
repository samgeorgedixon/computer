`include "src/core.svh"

module GPU(

    inout wire [15:0] bus,

    ExpansionUnitCommon_if.unit xuCommon,
    ExpansionUnitSignals_if.unit xuSignals

    );

    import "DPI-C" function void WriteDisplayPixel_256x256px_5bxRGB(input int pixelAddress, input int colour, input int reset);

    always @(posedge xuCommon.clk) begin
        
        if (xuCommon.r) begin
            WriteDisplayPixel_256x256px_5bxRGB(0, 0, 1);
        end else if (xuSignals.we) begin
            int pixelAddress =  { 8'b0, xuCommon.addr };
            int colour =        { 17'b0, bus[14:0] };
            int reset =         { 31'b0, bus[15] };

            WriteDisplayPixel_256x256px_5bxRGB(pixelAddress, colour, reset);
        end

    end

endmodule
