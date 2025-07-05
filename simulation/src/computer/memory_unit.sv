module MemoryUnit(

    input clk, r,
    inout [15:0] bus,
    input ri, rbi, ro, rbo,

    input [15:0] addr, saddr,
    output reg [23:0] addro,

    input string biosFilePath

    );

    assign addro = { 8'b0, addr } + { saddr, 8'b0 };
    assign bus = ro|rbo ? addro < 256 ? rbo ? { 8'b0, biosROM[addro[7:0]] } : { biosROM[addro[7:0]], biosROM[addro[7:0] + 1] } : ramInOut : 16'bz;

    wire [15:0] ramInOut;

    reg [7:0] biosROM [0:(2**8) - 1];
    reg [7:0] ram [0:(2**24) - 1];

    assign ramInOut = ro ? { ram[addro], ram[addro + 1] } : rbo ? { 8'b0, ram[addro] } : 16'bz; // RO

    assign ramInOut = addro > 'hff ? ri ? bus : rbi ? { 8'b0, bus[7:0] } : 16'bz : 16'bz; // RI

    import "DPI-C" function int LoadROMFile(input string filePath);
    import "DPI-C" function byte GetROMFileByte(input int index);
    import "DPI-C" function void CloseROMFile();

    initial begin
        
        int size = LoadROMFile(biosFilePath);
        
        if (size != 0) begin

            for (int i = 0; i < size; i = i + 1) begin
                biosROM[i] = GetROMFileByte(i);
            end

        end
        CloseROMFile();

        //$display("Memory Array:");
        //for (int i = 0; i < 2**4; i = i + 1) begin
        //    $display("memory_array[%0d] = %b", i, biosROM[i]);
        //end
        
    end

    always @(posedge clk) begin

        if (r) begin
            integer i;
            for (i = 0; i < 2**24; i = i + 1) begin
                ram[i] = 8'b0;
            end
        end else if (ri) begin
            ram[addro + 1] <= ramInOut[7:0];
            ram[addro] <= ramInOut[15:8];
        end else if (rbi) begin
            ram[addro] <= ramInOut[7:0];
        end

    end

endmodule
