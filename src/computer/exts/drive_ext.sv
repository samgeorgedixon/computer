module DriveExt(

    input clk, r,

    inout [15:0] bus,
    input [23:0] addr,

    input we, oe,
    
    input string filePath

    );

    reg [7:0] drive [0:(2**24)-1];

    assign bus = oe ? { drive[addr], drive[addr + 1] } : 16'bz;

    import "DPI-C" function int LoadROMFile(input string filePath);
    import "DPI-C" function byte GetROMFileByte(input int index);
    import "DPI-C" function void CloseROMFile();

    initial begin
        
        int size = LoadROMFile(filePath);
        
        if (size != 0) begin

            for (int i = 0; i < size; i = i + 1) begin
                drive[i][7:0] = GetROMFileByte(i);
            end

        end
        CloseROMFile();

    end

    always @(posedge clk) begin

        if (we) begin
            drive[addr + 1] <= bus[7:0];
            drive[addr] <= bus[15:8];
        end

    end

endmodule
