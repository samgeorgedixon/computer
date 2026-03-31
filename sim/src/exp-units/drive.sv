`include "src/core.svh"

module Drive_8bx24b(

    input logic clk, r, byte_low,

    inout wire [15:0] bus,
    input logic [23:0] addr,

    ExpansionSignals_if.unit driveUnitSignals,

    input string driveFilePath

    );

    logic [7:0] drive [0:(2**24) - 1];

    assign bus = driveUnitSignals.oe ? byte_low ? { 8'd0, drive[addr] } : { drive[addr], drive[addr + 1] } : 16'bz; // TODO: Need to Impliment Byte BUS

    always @(posedge clk) begin

        if (driveUnitSignals.we) begin
            if (byte_low) begin
                drive[addr] <= bus[7:0];
            end else begin
                drive[addr] <= bus[15:8];
                drive[addr + 1] <= bus[7:0];
            end
        end

    end

    import "DPI-C" function int LoadMemoryFile(input string filePath);
    import "DPI-C" function byte GetMemoryFileByte(input int index);
    import "DPI-C" function void CloseMemoryFile();

    initial begin
        
        int size = LoadMemoryFile(driveFilePath);
        
        if (size != 0) begin

            for (int i = 0; i < size; i = i + 1) begin
                drive[i] = GetMemoryFileByte(i);
            end

        end
        CloseMemoryFile();

        $display("Memory Array:");
        for (int i = 256; i < 512; i = i + 1) begin
            $display("memory_array[%0d] = %b", i, drive[i]);
        end
        
    end

endmodule
