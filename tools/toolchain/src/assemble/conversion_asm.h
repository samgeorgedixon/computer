#pragma once
#include <string>
#include <vector>

#include "assemble.h"

// Instructions
enum OpCode {
    NOP = 0, MOV, MOVI, LEA,
    LDW, LDWR, LDWP, LDB, LDBR, LDBP, STW, STWR, STWP, STB, STBR, STBP, LDXW, LDXWR, LDXWP, LDXB, LDXBR, LDXBP, STXW, STXWR, STXWP, STXB, STXBR, STXBP,
    JMP, JMPF, BZ, BNZ, BC, BNC, BS, BNS, BO, BNO,
    ADD, SUB, INC, INC2, DEC, DEC2, NOT, AND, OR, XOR, SLL, SRL, SRA, NEG, CMP,
    PUSH, PUSHB, POP, POPB, CALL, CALLF, RET, RETF
};
enum ParameterIndex {
    OP0 = 0, OP1, OP2, IMM
};

struct Instruction {
    int index;
    int byteSize;
    std::vector<ParameterIndex> parameters;
};

inline std::unordered_map<std::string, Instruction> instructions {
    { "nop",   { NOP,   2, {} } },
    { "mov",   { MOV,   2, { OP1, OP2 } } },
    { "movi",  { MOVI,  4, { OP1, IMM } } },
    { "lea",   { LEA,   4, { OP1, OP2, IMM } } },

    { "ldw",   { LDW,   4, { OP0, OP1, OP2, IMM } } },
    { "ldw+",  { LDWP,  2, { OP0, OP1, OP2 } } },
    { "ldwb",  { LDW,   4, { OP0, OP1, OP2, IMM } } },
    { "ldwb+", { LDWP,  2, { OP0, OP1, OP2 } } },
    { "stw",   { STW,   4, { OP0, OP1, OP2, IMM } } },
    { "stw+",  { STWP,  2, { OP0, OP1, OP2 } } },
    { "stwb",  { STW,   4, { OP0, OP1, OP2, IMM } } },
    { "stwb+", { STWP,  2, { OP0, OP1, OP2 } } },

    { "ldxw",  { LDXW,  4, { OP0, OP1, OP2, IMM } } },
    { "ldxw+", { LDXWP, 2, { OP0, OP1, OP2 } } },
    { "ldxb",  { LDXW,  4, { OP0, OP1, OP2, IMM } } },
    { "ldxb+", { LDXWP, 2, { OP0, OP1, OP2 } } },
    { "stxw",  { STXW,  4, { OP0, OP1, OP2, IMM } } },
    { "stxw+", { STXWP, 2, { OP0, OP1, OP2 } } },
    { "stxb",  { STXW,  4, { OP0, OP1, OP2, IMM } } },
    { "stxb+", { STXWP, 2, { OP0, OP1, OP2 } } },

    { "jmp",   { JMP,   4, { OP2, IMM } } },
    { "jmpf",  { JMPF,  4, { OP1, OP2, IMM } } },
    { "bz",    { BZ,    4, { OP2, IMM } } },
    { "bnz",   { BNZ,   4, { OP2, IMM } } },
    { "bc",    { BC,    4, { OP2, IMM } } },
    { "bnc",   { BNC,   4, { OP2, IMM } } },
    { "bs",    { BS,    4, { OP2, IMM } } },
    { "bns",   { BNS,   4, { OP2, IMM } } },
    { "bo",    { BO,    4, { OP2, IMM } } },
    { "bno",   { BNO,   4, { OP2, IMM } } },

    { "add",   { ADD,   2, { OP0, OP1, OP2 } } },
    { "sub",   { SUB,   2, { OP0, OP1, OP2 } } },
    { "inc",   { INC,   2, { OP0, OP1 } } },
    { "inc2",  { INC2,  2, { OP0, OP1 } } },
    { "dec",   { DEC,   2, { OP0, OP1 } } },
    { "dec2",  { DEC2,  2, { OP0, OP1 } } },
    { "not",   { NOT,   2, { OP0, OP1, OP2 } } },
    { "and",   { AND,   2, { OP0, OP1, OP2 } } },
    { "or",    { OR,    2, { OP0, OP1, OP2 } } },
    { "xor",   { XOR,   2, { OP0, OP1, OP2 } } },
    { "sll",   { SLL,   2, { OP0, OP1, OP2 } } },
    { "srl",   { SRL,   2, { OP0, OP1, OP2 } } },
    { "sra",   { SRA,   2, { OP0, OP1, OP2 } } },
    { "neg",   { NEG,   2, { OP0, OP1, OP2 } } },
    { "cmp",   { CMP,   2, { OP1, OP2 } } },

    { "push",  { PUSH,  2, { OP1 } } },
    { "pushb", { PUSH,  2, { OP1 } } },
    { "pop",   { POP,   2, { OP1 } } },
    { "popb",  { POP,   2, { OP1 } } },
    { "call",  { CALL,  4, { OP2, IMM } } },
    { "callf", { CALLF, 4, { OP1, OP2, IMM } } },
    { "ret",   { RET,   2, { } } },
    { "retf",  { RETF,  2, { } } },
};

// Operands
enum Register {
    ZERO = 0, R1, R2, R3, R4, SP, BP, CS, DS, SS, ES, IP, IR, AR
};
enum ExpansionUnit { // XU1: Memory, XU2: Drive
    XU1 = 0, XU2, XU3, XU4, XU5, XU6, XU7, XU8, XU9, XU10, XU11, XU12, XU13, XU14, XU15, XU16
};
enum SegmentIndex {
    CSS = 1, DSS, SSS, ESS
};

static std::unordered_map<std::string, int> instrParamConv {
    {"z", ZERO},

    {"r1", R1},
    {"r2", R2},
    {"r3", R3},
    {"r4", R4},

    {"sp", SP},
    {"bp", BP},

    {"cs", CS},
    {"ds", DS},
    {"ss", SS},
    {"es", ES},

    {"ip", IP},
    {"ir", IR},
    {"ar", AR},

    {"xu1", XU1},
    {"xu2", XU2},
    {"xu3", XU3},
    {"xu4", XU4},
    {"xu5", XU5},
    {"xu6", XU6},
    {"xu7", XU7},
    {"xu8", XU8},
    {"xu9", XU9},
    {"xu10", XU10},
    {"xu11", XU11},
    {"xu12", XU12},
    {"xu13", XU13},
    {"xu14", XU14},
    {"xu15", XU15},
    {"xu16", XU16},

    {"c", CSS},
    {"d", DSS},
    {"s", SSS},
    {"e", ESS}
};

uint16_t ConvertInstrParam(AssembleState& assembleState, std::string instrParamStr);

int ConvertLineInstruction(AssembleState& assembleState, const std::vector<std::string>& line);
