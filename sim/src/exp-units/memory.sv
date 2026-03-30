`include "src/core.svh"

module ROM_8bx8b(

    input logic clk, r,

    inout wire [15:0] bus,
    input logic [7:0] addr,

    input logic we, oe,

    input string romFilePath

    );

    logic [7:0] rom [0:(2**8) - 1];

    assign bus = oe ? { rom[addr], rom[addr + 1] } : 16'bz; // TODO: Need to Impliment Byte BUS
    
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
        //for (int i = 0; i < 2**8; i = i + 1) begin
        //    $display("memory_array[%0d] = %b", i, rom[i]);
        //end
        
    end

endmodule

module RAM_8bx24b(

    input logic clk, r,

    inout wire [15:0] bus,
    input logic [23:0] addr,

    input logic we, oe

    );

    logic [7:0] ram [0:(2**24) - 1];

    assign bus = oe ? { ram[addr], ram[addr + 1] } : 16'bz; // TODO: Need to Impliment Byte BUS

    always @(posedge clk) begin

        if (r) begin
            integer i;
            for (i = 0; i < 2**24; i = i + 1) begin
                ram[i] = 8'b0;
            end
        end else if (we) begin
            ram[addr] <= bus[15:8];
            ram[addr + 1] <= bus[7:0];
        end

    end

endmodule

module Memory (

    input logic clk, r,

    inout wire [15:0] bus,
    input logic [23:0] addr,

    ExpansionSignals_if.unit memoryUnitSignals,

    input string romFilePath

);

    logic rom_we, rom_oe;
    logic ram_we, ram_oe;

    assign rom_we = addr[23:8] == 16'd0 ? memoryUnitSignals.we : 1'd0;
    assign rom_oe = addr[23:8] == 16'd0 ? memoryUnitSignals.oe : 1'd0;

    assign ram_we = addr[23:8] != 16'd0 ? memoryUnitSignals.we : 1'd0;
    assign ram_oe = addr[23:8] != 16'd0 ? memoryUnitSignals.oe : 1'd0;

    ROM_8bx8b   rom(.clk(clk), .r(r), .bus(bus), .addr(addr[7:0]), .we(rom_we), .oe(rom_oe), .romFilePath(romFilePath));
    RAM_8bx24b  ram(.clk(clk), .r(r), .bus(bus), .addr(addr)     , .we(ram_we), .oe(ram_oe));
    
endmodule
