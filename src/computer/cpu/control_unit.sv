module ControlUnit(
    
    input clk, r,
    input [15:0] instr, input [7:0] flags,

    output [3:0] rsi, rso,

    output [3:0] aluOpSel, // S0 - S3
    output [3:0] aluASel, // AS0 - AS3
    output [3:0] aluBSel, // BS0 - BS0

    output [2:0] segSel, // SSO0 - SSO2

    output R1I ,
    output R1O ,
    output R2I ,
    output R2O ,
    output R3I ,
    output R3O ,
    output R4I ,
    output R4O ,
    output SPI ,
    output SPO ,
    output SBI ,
    output SBO ,
    output CSI ,
    output CSO ,
    output DSI ,
    output DSO ,
    output SSI ,
    output SSO ,
    output ESI ,
    output ESO ,
    output II  ,
    output IO  ,
    output MI  ,
    output MO  ,
    output RI  ,
    output RO  ,
    output PCE ,
    output PCI ,
    output PCO ,
    output SO  ,
    output FI  ,

    output OS1I,
    output OS1O,
    output OS1A,
    output OS2I,
    output OS2O,
    output OS2B,

    output OSLI,
    output OSLO,

    output E1I,
    output E1O,
    output E2I,
    output E2O,
    output E3I,
    output E3O,
    output E4I,
    output E4O,

    input string cpuInstrRomFilePath

    );

    reg[3:0] microCodeIndex = 0;
    reg [63:0] microCodeROM [0:(2**12)-1];

    /* verilator lint_off UNOPTFLAT */
    wire [11:0] addr;
    /* verilator lint_on UNOPTFLAT */

    // Set All Control Signals
    assign R1I  = microCodeROM[addr][0];
    assign R1O  = microCodeROM[addr][1];
    assign R2I  = microCodeROM[addr][2];
    assign R2O  = microCodeROM[addr][3];
    assign R3I  = microCodeROM[addr][4];
    assign R3O  = microCodeROM[addr][5];
    assign R4I  = microCodeROM[addr][6];
    assign R4O  = microCodeROM[addr][7];
    assign SPI  = microCodeROM[addr][8];
    assign SPO  = microCodeROM[addr][9];
    assign SBI  = microCodeROM[addr][10];
    assign SBO  = microCodeROM[addr][11];
    assign CSI  = microCodeROM[addr][12];
    assign CSO  = microCodeROM[addr][13];
    assign DSI  = microCodeROM[addr][14];
    assign DSO  = microCodeROM[addr][15];
    assign SSI  = microCodeROM[addr][16];
    assign SSO  = microCodeROM[addr][17];
    assign ESI  = microCodeROM[addr][18];
    assign ESO  = microCodeROM[addr][19];
    assign II   = microCodeROM[addr][20];
    assign IO   = microCodeROM[addr][21];
    assign MI   = microCodeROM[addr][22];
    assign MO   = microCodeROM[addr][23];
    assign RI   = microCodeROM[addr][24];
    assign RO   = microCodeROM[addr][25];
    assign PCE  = microCodeROM[addr][26];
    assign PCI  = microCodeROM[addr][27];
    assign PCO  = microCodeROM[addr][28];
    assign SO   = microCodeROM[addr][29];
    assign FI   = microCodeROM[addr][30];
    
    assign OS1I = microCodeROM[addr][46];
    assign OS1O = microCodeROM[addr][47];
    assign OS1A = microCodeROM[addr][48];
    assign OS2I = microCodeROM[addr][49];
    assign OS2O = microCodeROM[addr][50];
    assign OS2B = microCodeROM[addr][51];
    assign OSLI = microCodeROM[addr][52];
    assign OSLO = microCodeROM[addr][53];

    assign aluOpSel = microCodeROM[addr][34:31];
    assign aluASel = OS1A ? instr[7:4] : microCodeROM[addr][38:35];
    assign aluBSel = OS2B ? instr[3:0] : microCodeROM[addr][42:39];

    assign segSel = microCodeROM[addr][45:43];

    // Set All the Selects    
    assign addr[3:0] = microCodeIndex;
    assign addr[9:4] = instr[15:10];
    assign addr[11:10] = flags[1:0];

    assign rsi = OS1I ? instr[7:4] : OS2I ? instr[3:0] : 4'bz;
    assign rso = OS1O ? instr[7:4] : OS2O ? instr[3:0] : 4'bz;

    wire [2:0] operandSelect3b;
    wire [2:0] operandSelect;
    wire [1:0] operandSelectJob;

    wire [2:0] esi, eso;
    
    assign operandSelect3b = {1'bz, instr[9:8]};
    assign operandSelect = operandSelect3b + 1;
    assign operandSelectJob = microCodeROM[addr][55:54];

    assign segSel   = operandSelectJob == 1 ? OSLO ? operandSelect : 3'bz : 3'bz;
    assign esi      = operandSelectJob == 2 ? OSLI ? operandSelect : 3'bz : 3'bz;
    assign eso      = operandSelectJob == 2 ? OSLO ? operandSelect : 3'bz : 3'bz;
    assign rsi      = operandSelectJob == 3 ? OSLI ? {1'bz, operandSelect} : 4'bz : 4'bz;
    assign rso      = operandSelectJob == 3 ? OSLO ? {1'bz, operandSelect} : 4'bz : 4'bz;

    assign E1I = esi == 1 ? 1 : 0; assign E1O = eso == 1 ? 1 : 0;
    assign E2I = esi == 2 ? 1 : 0; assign E2O = eso == 2 ? 1 : 0;
    assign E3I = esi == 3 ? 1 : 0; assign E3O = eso == 3 ? 1 : 0;
    assign E4I = esi == 4 ? 1 : 0; assign E4O = eso == 4 ? 1 : 0;

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
