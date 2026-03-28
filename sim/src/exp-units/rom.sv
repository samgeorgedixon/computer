module ROM_8bx8b(

    input logic clk, r,

    inout wire [15:0] bus,
    input logic [23:0] addr,

    ExpansionSignals_if.unit romUnitSignals,

    input string romFilePath

    );

    logic [7:0] rom [0:(2**8) - 1];

    assign bus = romUnitSignals.oe ? { rom[addr[7:0]], rom[addr[7:0] + 1] } : 16'bz; // TODO: Need to Impliment Byte BUS
    
    import "DPI-C" function int LoadMemoryFile(input string filePath);
    import "DPI-C" function byte GetMemoryFileByte(input int index);
    import "DPI-C" function void CloseMemoryFile();

    initial begin
        
        int size = LoadMemoryFile(romFilePath);
        
        if (size != 0) begin

            for (int i = 0; i < size; i = i + 1) begin
                rom[i] = GetMemoryFileByte(i);
            end

        end
        CloseMemoryFile();

        //$display("Memory Array:");
        //for (int i = 0; i < 2**4; i = i + 1) begin
        //    $display("memory_array[%0d] = %b", i, rom[i]);
        //end
        
    end

endmodule
