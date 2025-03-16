`include "src/ram.sv"

module MemoryUnit(

    input clk, r,
    inout [15:0] bus,
    input rwe, roe,

    input [15:0] addr, saddr,
    output reg [23:0] addro

    );

    assign addro = { 8'b0, addr } + { saddr, 8'b0 };

    wire [15:0] ramInOut;
    RAM16B24A ram(clk, r, rwe, roe, addro, ramInOut);

    reg [15:0] biosROM [0:(2**4)-1];

    assign bus = roe ? addro < 16 ? biosROM[addro[3:0]] : ramInOut : 16'bz;
    assign ramInOut = rwe ? addro > 15 ? bus : 16'bz : 16'bz;

    import "DPI-C" function int LoadROMFile(input string filePath);
    import "DPI-C" function byte GetROMFileByte(input int index);
    import "DPI-C" function void CloseROMFile();

    initial begin
        
        int size = LoadROMFile("bios.bin");
        
        if (size != 0) begin

            int j = 0;
            for (int i = 0; i < size; i = i + 2) begin
                
                biosROM[j][15:8] = GetROMFileByte(i + 0);
                biosROM[j][7:0] = GetROMFileByte(i + 1);

                j += 1;
                
            end

        end
        CloseROMFile();

        //$display("Memory Array:");
        //for (int i = 0; i < 2**4; i = i + 1) begin
        //    $display("memory_array[%0d] = %b", i, biosROM[i]);
        //end
        
    end

endmodule
