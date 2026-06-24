`include "src/core.svh"

module ROM_8bx8b(

    ExpansionUnitCommon_if.unit xuCommon,

    inout wire [15:0] bus,

    input logic [7:0] addr,
    input logic we, oe,

    input string romFilePath

    );

    logic [7:0] rom [0:(2**8) - 1];

    assign bus = oe ? xuCommon.byte_low ? { 8'd0, rom[addr] } : { rom[addr], rom[addr + 1] } : 16'd0;
    
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

        $display("Memory Array:");
        for (int i = 0; i < 2**8; i = i + 1) begin
            $display("memory_array[%0d] = %b", i, rom[i]);
        end
        
    end

endmodule

module RAM_8bx24b(

    ExpansionUnitCommon_if.unit xuCommon,

    inout wire [15:0] bus,

    input logic [23:0] addr,
    input logic we, oe

    );

    logic [7:0] ram [0:(2**24) - 1];

    assign bus = oe ? xuCommon.byte_low ? { 8'd0,  ram[addr] } : { ram[addr], ram[addr + 1] } : 16'd0;

    always @(posedge xuCommon.clk) begin

        if (xuCommon.r) begin
            integer i;
            for (i = 0; i < 2**24; i = i + 1) begin
                ram[i] = 8'b0;
            end
        end else if (we) begin
            if (xuCommon.byte_low) begin
                ram[addr] <= bus[7:0];
            end else begin
                ram[addr] <= bus[15:8];
                ram[addr + 1] <= bus[7:0];
            end
        end

    end

endmodule

module Memory (

    inout wire [15:0] bus,

    ExpansionUnitCommon_if.unit xuCommon,
    ExpansionUnitSignals_if.unit xuSignals,

    input string romFilePath

);

    logic rom_we, rom_oe;
    logic ram_we, ram_oe;

    assign rom_we = xuCommon.addr[23:8] == 16'd0 ? xuSignals.we : 1'd0;
    assign rom_oe = xuCommon.addr[23:8] == 16'd0 ? xuSignals.oe : 1'd0;

    assign ram_we = xuCommon.addr[23:8] != 16'd0 ? xuSignals.we : 1'd0;
    assign ram_oe = xuCommon.addr[23:8] != 16'd0 ? xuSignals.oe : 1'd0;

    ROM_8bx8b   rom(xuCommon, bus, xuCommon.addr[7:0], rom_we, rom_oe, romFilePath);
    RAM_8bx24b  ram(xuCommon, bus, xuCommon.addr     , ram_we, ram_oe);
    
endmodule
