module RegistersUnit(

    input clk, r,
    inout [15:0] bus,

    input [3:0] rsi, rso,

    input r1i, r1o,
    input r2i, r2o,
    input r3i, r3o,
    input r4i, r4o,

    input cswe, csoe,
    input dswe, dsoe,
    input sswe, ssoe,
    input eswe, esoe,

    input [2:0] sso,

    input iwe, ioe,
    input mwe, moe,

    input spwe, spoe,
    input sbwe, sboe,

    output [15:0] ao, bo, co, do_, mo, io, spo, sbo, csro, ssro, dsro, esro, so

    );

    Reg16 regA(clk, r, r1i | rsi == 1 ? 1 : 0, r1o | rso == 1 ? 1 : 0, bus, bus, ao);
    Reg16 regB(clk, r, r2i | rsi == 2 ? 1 : 0, r2o | rso == 2 ? 1 : 0, bus, bus, bo);
    Reg16 regC(clk, r, r3i | rsi == 3 ? 1 : 0, r3o | rso == 3 ? 1 : 0, bus, bus, co);
    Reg16 regD(clk, r, r4i | rsi == 4 ? 1 : 0, r4o | rso == 4 ? 1 : 0, bus, bus, do_);

    assign so = sso == 1 ? csro : sso == 2 ? dsro : sso == 3 ? ssro : sso == 4 ? esro : 0;

    Reg16 regCS(clk, r, cswe | rsi == 7 ? 1 : 0,  csoe | rso == 7 ? 1 : 0,  bus, bus, csro);
    Reg16 regDS(clk, r, dswe | rsi == 8 ? 1 : 0,  dsoe | rso == 8 ? 1 : 0,  bus, bus, dsro);
    Reg16 regSS(clk, r, sswe | rsi == 9 ? 1 : 0,  ssoe | rso == 9 ? 1 : 0, bus, bus, ssro);
    Reg16 regES(clk, r, eswe | rsi == 10 ? 1 : 0, esoe | rso == 10 ? 1 : 0, bus, bus, esro);

    wire [15:0] memOut;
    wire [15:0] instrOut;
    assign bus[9:0] = instrOut[9:0];

    Reg16 regMemory         (clk, r, mwe, moe, bus, memOut, mo);
    Reg16 regInstr          (clk, r, iwe, ioe, bus, instrOut, io);

    Reg16 regStackPointer   (clk, r, spwe | rsi == 5 ? 1 : 0, spoe | rso == 5 ? 1 : 0, bus, bus, spo);
    Reg16 regStackBase      (clk, r, sbwe | rsi == 6 ? 1 : 0, sboe | rso == 6 ? 1 : 0, bus, bus, sbo);

endmodule
