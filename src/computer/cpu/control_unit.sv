module ControlUnit(
    
    input clk, r,
    input [15:0] instr, input [7:0] flags,

    output [3:0] rsbi, rsbo, rsti, rsto,

    output AI,
    output AO,
    output BI,
    output BO,
    output CI,
    output CO,
    output DI,
    output DO,
    output EI,
    output EO,
    output II,
    output IO,
    output MI,
    output RI,
    output RO,
    output PCE,
    output PCI,
    output PCO,
    output SO,
    output S0,
    output S1,
    output S2,
    output FI,
    output AEI,
    output AEO,
    output BEI,
    output BEO,
    output CEI,
    output CEO,
    output DEI,
    output DEO,
    output SPI,
    output SPO,
    output SBI,
    output SBO,
    output CSI,
    output CSO,
    output DSI,
    output SSI,
    output ESI,
    output SSO0,
    output SSO1,
    output SSO2,
    output SSO,
    output RSBI,
    output RSBO,
    output RSTI,
    output RSTO,

    input string cpuInstrRomFilePath

    );

    reg[3:0] microCodeIndex = 0;
    reg [63:0] microCodeROM [0:(2**12)-1];

    /* verilator lint_off UNOPTFLAT */
    wire [11:0] addr;
    /* verilator lint_on UNOPTFLAT */

    assign AI = microCodeROM[addr][0];
    assign AO = microCodeROM[addr][1];
    assign BI = microCodeROM[addr][2];
    assign BO = microCodeROM[addr][3];
    assign CI = microCodeROM[addr][4];
    assign CO = microCodeROM[addr][5];
    assign DI = microCodeROM[addr][6];
    assign DO = microCodeROM[addr][7];
    assign EI = microCodeROM[addr][8];
    assign EO = microCodeROM[addr][9];
    assign II = microCodeROM[addr][10];
    assign IO = microCodeROM[addr][11];
    assign MI = microCodeROM[addr][12];
    assign RI = microCodeROM[addr][13];
    assign RO = microCodeROM[addr][14];
    assign PCE = microCodeROM[addr][15];
    assign PCI = microCodeROM[addr][16];
    assign PCO = microCodeROM[addr][17];
    assign SO = microCodeROM[addr][18];
    assign S0 = microCodeROM[addr][19];
    assign S1 = microCodeROM[addr][20];
    assign S2 = microCodeROM[addr][21];
    assign FI = microCodeROM[addr][22];
    assign AEI = microCodeROM[addr][23];
    assign AEO = microCodeROM[addr][24];
    assign BEI = microCodeROM[addr][25];
    assign BEO = microCodeROM[addr][26];
    assign CEI = microCodeROM[addr][27];
    assign CEO = microCodeROM[addr][28];
    assign DEI = microCodeROM[addr][29];
    assign DEO = microCodeROM[addr][30];
    assign SPI = microCodeROM[addr][31];
    assign SPO = microCodeROM[addr][32];
    assign SBI = microCodeROM[addr][33];
    assign SBO = microCodeROM[addr][34];
    assign CSI = microCodeROM[addr][35];
    assign CSO = microCodeROM[addr][36];
    assign DSI = microCodeROM[addr][37];
    assign SSI = microCodeROM[addr][38];
    assign ESI = microCodeROM[addr][39];
    assign SSO0 = microCodeROM[addr][40];
    assign SSO1 = microCodeROM[addr][41];
    assign SSO2 = microCodeROM[addr][42];
    assign SSO = microCodeROM[addr][43];
    assign RSBI = microCodeROM[addr][44];
    assign RSBO = microCodeROM[addr][45];
    assign RSTI = microCodeROM[addr][46];
    assign RSTO = microCodeROM[addr][47];

    assign addr[3:0] = microCodeIndex;
    assign addr[9:4] = instr[15:10];
    assign addr[11:10] = flags[1:0];

    assign rsbi = RSBI ? instr[3:0] : 4'bz;
    assign rsbo = RSBO ? instr[3:0] : 4'bz;
    assign rsti = RSTI ? instr[7:4] : 4'bz;
    assign rsto = RSTO ? instr[7:4] : 4'bz;

    reg [2:0] segmentIndex;
    assign segmentIndex = instr[9:8] + 1;

    assign SSO0 = SSO ? segmentIndex[0] : 1'bz;
    assign SSO1 = SSO ? segmentIndex[1] : 1'bz;
    assign SSO2 = SSO ? segmentIndex[2] : 1'bz;

    import "DPI-C" function int LoadROMFile(input string filePath);
    import "DPI-C" function byte GetROMFileByte(input int index);
    import "DPI-C" function void CloseROMFile();

    initial begin
        
        int size = LoadROMFile(cpuInstrRomFilePath);
        
        if (size != 0) begin

            int j = 0;
            for (int i = 0; i < size; i = i + 8) begin
                
                microCodeROM[j][63:56] = GetROMFileByte(i + 0);
                microCodeROM[j][55:48] = GetROMFileByte(i + 1);
                microCodeROM[j][47:40] = GetROMFileByte(i + 2);
                microCodeROM[j][39:32] = GetROMFileByte(i + 3);
                microCodeROM[j][31:24] = GetROMFileByte(i + 4);
                microCodeROM[j][23:16] = GetROMFileByte(i + 5);
                microCodeROM[j][15:8] = GetROMFileByte(i + 6);
                microCodeROM[j][7:0] = GetROMFileByte(i + 7);

                j += 1;
                
            end

            //$display("Memory Array:");
            //for (int i = 0; i < 100; i = i + 1) begin
            //    $display("memory_array[%0d] = %b", i, microCodeROM[i]);
            //end

        end
        CloseROMFile();
        
    end

    always @(posedge ~clk or posedge clk) begin

        if (~clk && !r) begin

            microCodeIndex <= microCodeIndex + 1;
    
            if (microCodeROM[addr + 1][63]) begin
                microCodeIndex <= 0;
            end

        end else if (r) begin
            microCodeIndex <= 0;
        end

    end

endmodule
