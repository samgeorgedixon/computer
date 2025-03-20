module GPU_Ext(

    input clk, r,

    inout [15:0] bus,
    input [23:0] addr,

    input we, oe

    );

    import "DPI-C" function void WriteDisplayPixel(input int pixelAddress, input int colour, input int reset);

    always @(posedge clk) begin
        
        if (r) begin
            WriteDisplayPixel(0, 0, 1);
        end else if (we) begin
            int pixelAddress =  { 8'b0, addr };
            int colour =        { 17'b0, bus[14:0] };
            int reset =         { 31'b0, bus[15] };

            WriteDisplayPixel(pixelAddress, colour, reset);
        end

    end

endmodule
