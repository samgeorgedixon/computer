#include "assembler.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cctype>
#include <bitset>
#include <fstream>

#include "core/core.h"
#include "core/parser.h"

std::vector<char> program;
std::unordered_map<std::string, unsigned int> labels;
int currentAddress = 0;

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

enum Register {
    ZERO = 0, R1, R2, R3, R4, SP, BP, CS, DS, SS, ES, IP, IR, AR
};
enum ExpansionUnit { // XU1: Memory, XU2: Drive
	XU1 = 0, XU2, XU3, XU4, XU5, XU6, XU7, XU8, XU9, XU10, XU11, XU12, XU13, XU14, XU15, XU16
};
enum SegmentIndex {
	CSS = 1, DSS, SSS, ESS
};

std::unordered_map<std::string, Instruction> instructions {
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

std::unordered_map<std::string, int> instrParamConv {
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

uint16_t ConvertInstrParam(std::string instrParamStr) {
    uint16_t instrParam = 0;
    
    if (instrParamStr[0] == '0' && instrParamStr[1] == 'x') {
        instrParam = std::stoi(instrParamStr.substr(2), nullptr, 16);
    }
    else if (instrParamStr[0] == '0' && instrParamStr[1] == 'b') {
        instrParam = std::stoi(instrParamStr.substr(2), nullptr, 2);
    }
    else if (IsStrAlpha(instrParamStr)) {
        auto it = instrParamConv.find(instrParamStr);

        if (it != instrParamConv.end()) { // Contains Key
            instrParam = instrParamConv[instrParamStr];
        }
        else {
            instrParam = labels[instrParamStr];
        }
    }
    else {
        instrParam = std::stoi(instrParamStr);
    }
    return instrParam;
}

int CheckLine(std::vector<std::string> line) {
    if (line[0] == "%") {
        if (line[1] == "org") {
            if (line.size() > 2) {
                currentAddress = ConvertInstrParam(line[2]);
            }
        }
        else if (line[1] == "dw") {
            return 2;
        }
        else if (line[1] == "db") {
            return 1;
        }
        else if (line[1] == "segment") {
            currentAddress = 0;
        }
        return 0;
    }
    else if (line[0] == ":") {
        labels[line[1]] = currentAddress;
        return 0;
    }

    auto it = instructions.find(line[0]);

    if (it != instructions.end()) { // Contains Key
        if (line.size() - 1 != instructions[line[0]].parameters.size() || instructions[line[0]].index == MOV) { // For Non Imm Versions or Imm Versions like LDWR or MOVI
            switch (instructions[line[0]].index) {
                case LDW:
                case LDB:
                case STW:
                case STB:
                case LDXW:
                case LDXB:
                case STXW:
                case STXB: {
                    return 2;
                }
                case MOV: {
                    std::string immParamToken = line[2];

                    if (immParamToken[0] == '0' && immParamToken[1] == 'x') {
                        return 4;
                    }
                    else if (immParamToken[0] == '0' && immParamToken[1] == 'b') {
                        return 4;
                    }
                    else if (!IsStrAlpha(immParamToken)) {
                        auto it = instrParamConv.find(immParamToken);

                        if (it == instrParamConv.end()) { // Does Not Contain Key
                            return 4;
                        }
                    }

                    break;
                }
                default: {
                    printf("Error: Less Tokens than Required for Instruction: %s, %s\n", line[0], line);
                    return 0;
                }
            }
        }

        return instructions[line[0]].byteSize;
    }
    else {
        printf("Error: Invalid Instruction Opcode: %s, %s\n", line[0], line);
        return 0;
    }
}

void SetLabels(const std::vector<std::vector<std::string>>& lines) {
    for (int i = 0; i < lines.size(); i++) {
        currentAddress += CheckLine(lines[i]);
    }
}

int ConvertLineInstruction(const std::vector<std::string>& line) {
    if (line[0] == "%") {
        if (line[1] == "org") {
            if (line.size() > 1) {
                currentAddress = ConvertInstrParam(line[2]);
            }
        }
        else if (line[1] == "dw") {
            while (program.size() <= currentAddress) {
                program.push_back(0);
            }
            if (line.size() > 1) {
                uint16_t instrParam = ConvertInstrParam(line[2]);

                program[currentAddress] = (instrParam & 0xff00) >> 8;
                program.push_back(instrParam & 0x00ff);
            }
            else {
                program[currentAddress] = 0;
                program.push_back(0);
            }
            return 2;
        }
        else if (line[1] == "db") {
            while (program.size() <= currentAddress) {
                program.push_back(0);
            }
            if (line.size() > 1) {
                program[currentAddress] = ConvertInstrParam(line[2]);
            }
            else {
                program[currentAddress] = 0;
            }
            return 1;
        }
        return 0;
    }

    std::string instrToken = line[0];

    uint8_t instr;
    auto it = instructions.find(instrToken);

    if (it != instructions.end()) { // Contains Key
        instr = instructions[instrToken].index;
    }
    else {
        printf("Error: Invalid Instruction Opcode: %s\n", instrToken.c_str());
        return 0;
    }
    uint16_t instrParam = 0;
    
    uint16_t extraParam = 0;
    bool extraParamOn = false;

    if (line.size() - 1 != instructions[instrToken].parameters.size() || instr == MOV) { // For Non Imm Versions or Imm Versions like LDWR or MOVI
        switch (instr) {
            case LDW:
            case LDB:
            case STW:
            case STB:
            case LDXW:
            case LDXB:
            case STXW:
            case STXB: {
                instr++;
                instrToken = "ldw+";

                break;
            }
            case MOV: {
                std::string immParamToken = line[2];

                if (immParamToken[0] == '0' && immParamToken[1] == 'x') {
                    instr++;
                    instrToken = "movi";

                    std::cout << "s\n";
                }
                else if (immParamToken[0] == '0' && immParamToken[1] == 'b') {
                    instr++;
                    instrToken = "movi";

                    std::cout << "s\n";
                }
                else if (!IsStrAlpha(immParamToken)) {
                    auto it = instrParamConv.find(immParamToken);

                    if (it == instrParamConv.end()) { // Does Not Contain Key
                        instr++;
                        instrToken = "movi";

                        std::cout << "s\n";
                    }
                }

                break;
            }
            default: {
                printf("Error: Less Tokens than Required for Instruction: %s\n", line[0].c_str());
                return 0;
            }
        }
    }

    for (int i = 0; i < instructions[instrToken].parameters.size(); i++) {
        if (instructions[instrToken].parameters[i] == ParameterIndex::OP0) {
            instrParam |= ((ConvertInstrParam(line[i + 1]) & 0b11) - 1 << 8);
        }
        else if (instructions[instrToken].parameters[i] == ParameterIndex::OP1) {
            instrParam |= ((ConvertInstrParam(line[i + 1]) & 0b1111) << 4);
        }
        else if (instructions[instrToken].parameters[i] == ParameterIndex::OP2) {
            instrParam |= (ConvertInstrParam(line[i + 1]) & 0b1111);
        }
        else if (instructions[instrToken].parameters[i] == ParameterIndex::IMM) {
            extraParam = ConvertInstrParam(line[i + 1]);
            extraParamOn = true;
        }
    }

    while (program.size() <= currentAddress) {
        program.push_back(0);
    }

    uint16_t instrWord = 0xFC00&(instr << 10)|0x03FF&(instrParam);
    program[currentAddress] = (instrWord & 0xff00) >> 8;
    program.push_back(instrWord & 0x00ff);

    if (extraParamOn) {
        program.push_back((extraParam & 0xff00) >> 8);
        program.push_back(extraParam & 0x00ff);
        return 4;
    }
    return 2;
}

void CreateProgram(const std::vector<std::vector<std::string>>& lines) {
    currentAddress = 0;

    for (int i = 0; i < lines.size(); i++) {
        if (lines[i][0] == ":") {
            continue;
        }
        currentAddress += ConvertLineInstruction(lines[i]);
    }
}

void PrintProgram() {
    printf("---\n");

    int skipped = 0;

    for (int i = 0; i < program.size(); i += 2) {
        if (i >= 2) {
            if (program[i - 2] == 0 && program[i - 1] == 0 && program[i] == 0 && program[i + 1] == 0) {
                skipped++;
                continue;
            }
        }
        if (skipped != 0) {
            printf("* - %i\n", skipped * 2);
            skipped = 0;
        }

        printf("%i: %s - %i: %s\n", i, std::bitset<8>(program[i]).to_string().c_str(), i + 1, std::bitset<8>(program[i + 1]).to_string().c_str());
    }
    printf("Bytes: %i\n", program.size());
}

void Preprosessor(std::vector<std::vector<std::string>>& lines) {
    std::unordered_map<std::string, std::vector<std::string>> defines;
    
    for (int i = 0; i < lines.size(); i++) {
        std::vector<std::string> line = lines[i];
        std::string str;
        
        if (line[0] == "%") {
            if (line[1] == "define") {
                std::vector<std::string> define(line.begin() + 3, line.end());
                
                defines[line[2]] = line;
            }
            else if (line[1] == "include") {
                std::string includePath = line[3];
                
                std::vector<std::vector<std::string>> asmFile = ParseASMFile(includePath);
                lines.insert(lines.begin() + i, asmFile.begin(), asmFile.end());
            }
        }
        
        for (int j = 0; j < line.size(); j++) {
            if (line[j] == "$") {
                std::vector<std::string> define = defines[line[j + 1]];

                line.insert(line.begin() + j + 2, define.begin(), define.end());

                line.erase(line.begin() + j, line.begin() + j + 2);
            }
        }
    }
    SetLabels(lines);
}

std::vector<char> AssembleLines(std::vector<std::vector<std::string>>& lines) {
    Preprosessor(lines);
    CreateProgram(lines);

    PrintProgram();

    return program;
}
