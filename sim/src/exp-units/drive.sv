`include "src/core.svh"

module Drive_8bx24b(

    input logic clk, r,

    inout wire [15:0] bus,
    input logic [23:0] addr,

    ExpansionSignals_if.unit driveUnitSignals,

    input string driveFilePath

    );

    logic [7:0] drive [0:(2**24) - 1];

    assign bus = driveUnitSignals.oe ? { drive[addr], drive[addr + 1] } : 16'bz; // TODO: Need to Impliment Byte BUS

    always @(posedge clk) begin

        if (r) begin
            integer i;
            for (i = 0; i < 2**24; i = i + 1) begin
                drive[i] = 8'b0;
            end
        end else if (driveUnitSignals.we) begin
            drive[addr] <= bus[15:8];
            drive[addr + 1] <= bus[7:0];
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

        //$display("Memory Array:");
        //for (int i = 0; i < 2**4; i = i + 1) begin
        //    $display("memory_array[%0d] = %b", i, drive[i]);
        //end
        
    end

endmodule
