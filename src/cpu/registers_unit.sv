module RegistersUnit(

    input clk, r,
    inout [15:0] bus,

    input [3:0] rsbo, rsto,
    input [3:0] rsbi, rsti,

    input awe, aoe, output [15:0] ao,

    input bwe, boe, output [15:0] bo,

    input cwe, coe, output [15:0] co,

    input dwe, doe, output [15:0] do_,

    input ewe, eoe, output [15:0] eo,

    input cswe, csoe,
    input dswe,
    input sswe,
    input eswe,

    input [2:0] sso, output [15:0] so,

    input mwe, output [15:0] mo,
    input iwe, ioe, output [15:0] io,

    input spwe, spoe, output [15:0] spo,
    input sbwe, sboe, output [15:0] sbo

    );

    wire [3:0] rso = rsbo | rsto;
    wire [3:0] rsi = rsbi | rsti;

    Reg16 regA(clk, r, awe | rsi == 1 ? 1 : 0, aoe | rso == 1 ? 1 : 0, bus, bus, ao);
    Reg16 regB(clk, r, bwe | rsi == 2 ? 1 : 0, boe | rso == 2 ? 1 : 0, bus, bus, bo);
    Reg16 regC(clk, r, cwe | rsi == 3 ? 1 : 0, coe | rso == 3 ? 1 : 0, bus, bus, co);
    Reg16 regD(clk, r, dwe | rsi == 4 ? 1 : 0, doe | rso == 4 ? 1 : 0, bus, bus, do_);
    Reg16 regE(clk, r, ewe | rsi == 5 ? 1 : 0, eoe | rso == 5 ? 1 : 0, bus, bus, eo);

    wire [15:0] csro = 0;
    wire [15:0] dsro = 0;
    wire [15:0] ssro = 0;
    wire [15:0] esro = 0;

    assign so = sso == 0 ? 0 : sso == 1 ? csro : sso == 2 ? dsro : sso == 3 ? ssro : sso == 4 ? esro : 0;

    Reg16 regCS(clk, r, cswe | rsi == 8 ? 1 : 0,  csoe | rso == 8 ? 1 : 0,  bus, bus, csro);
    Reg16 regDS(clk, r, dswe | rsi == 9 ? 1 : 0,  rsi == 9 ? 1 : 0,         bus, bus, dsro);
    Reg16 regSS(clk, r, sswe | rsi == 10 ? 1 : 0, rsi == 10 ? 1 : 0,         bus, bus, ssro);
    Reg16 regES(clk, r, eswe | rsi == 11 ? 1 : 0, rsi == 11 ? 1 : 0,         bus, bus, esro);

    wire [15:0] memOut;
    wire [15:0] instrOut;
    assign bus[9:0] = instrOut[9:0];

    Reg16 regMemory         (clk, r, mwe, 0, bus, memOut, mo);
    Reg16 regInstr          (clk, r, iwe, ioe, bus, instrOut, io);

    Reg16 regStackPointer   (clk, r, spwe | rsi == 6 ? 1 : 0, spoe | rso == 6 ? 1 : 0, bus, bus, spo);
    Reg16 regStackBase      (clk, r, sbwe | rsi == 7 ? 1 : 0, sboe | rso == 7 ? 1 : 0, bus, bus, sbo);

endmodule
