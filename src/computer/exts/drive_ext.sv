module DriveExt(

    input clk, r,

    inout [15:0] bus,
    input [23:0] addr,

    input we, oe

    );

    reg [15:0] drive [0:(2**24)-1];

    assign bus = oe ? drive[addr] : 16'bz;

    import "DPI-C" function int LoadROMFile(input string filePath);
    import "DPI-C" function byte GetROMFileByte(input int index);
    import "DPI-C" function void CloseROMFile();

    initial begin
        
        int size = LoadROMFile("gpu_test.bin");
        
        if (size != 0) begin

            int j = 0;
            for (int i = 0; i < size; i = i + 2) begin
                
                drive[j][15:8] = GetROMFileByte(i + 0);
                drive[j][7:0] = GetROMFileByte(i + 1);

                j += 1;
                
            end

        end
        CloseROMFile();

    end

    always @(posedge clk) begin

        if (we) begin
            drive[addr] <= bus;
        end

    end

endmodule
