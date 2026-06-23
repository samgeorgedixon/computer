#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cctype>
#include <bitset>
#include <fstream>

#include "assembler.h"

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
    R1 = 1, R2, R3, R4, SP, BP, CS, DS, SS, ES, ZERO, IP, IR, AR
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

	{"z", ZERO},

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

std::string TrimA(std::string str, std::string whitespace = " \t\r") {
    int strBegin = str.find_first_not_of(whitespace);

    if (strBegin == std::string::npos) {
        return "";
    }

    int strEnd = str.find_last_not_of(whitespace);
    int strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::vector<std::string> SplitA(std::string str, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(TrimA(item));
    }
    if (result.size() == 0 && TrimA(str).size() != 0) {
        result.push_back(TrimA(str));
    }

    return result;
}

bool IsStrAlphaA(std::string str) {
    bool strAlpha = false;
    for (int i = 0; i < str.length(); i++) {
        strAlpha = isalpha(str[i]);
        if (strAlpha)
            return strAlpha;
    }
    return strAlpha;
}

uint16_t ConvertInstrParam(std::string instrParamStr) {
    uint16_t instrParam = 0;
    
    if (instrParamStr[0] == '0' && instrParamStr[1] == 'x') {
        instrParam = std::stoi(instrParamStr.substr(2), nullptr, 16);
    }
    else if (instrParamStr[0] == '0' && instrParamStr[1] == 'b') {
        instrParam = std::stoi(instrParamStr.substr(2), nullptr, 2);
    }
    else if (IsStrAlphaA(instrParamStr)) {
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

int CheckLine(std::string line) {
    std::string str;
    std::stringstream ss(line);
    std::vector<std::string> lineTokens;

    while (getline(ss, str, ' ')) {
        lineTokens.push_back(str);
    }
    
    if (line[0] == '%') {
        if (lineTokens[0].substr(1) == "org") {
            if (lineTokens.size() > 1) {
                currentAddress = ConvertInstrParam(lineTokens[1]);
            }
        }
        else if (lineTokens[0].substr(1) == "dw") {
            return 2;
        }
        else if (lineTokens[0].substr(1) == "db") {
            return 1;
        }
        else if (lineTokens[0].substr(1) == "segment") {
            currentAddress = 0;
        }
        return 0;
    }
    else if (line[0] == ':') {
        labels[line.substr(line.find(":") + 1)] = currentAddress;
        return 0;
    }
    else if (line[0] == '/' && line[1] == '/') {
        return 0;
    }

    auto it = instructions.find(lineTokens[0]);

    if (it != instructions.end()) { // Contains Key

        if (lineTokens.size() - 1 != instructions[lineTokens[0]].parameters.size() || instructions[lineTokens[0]].index == MOV) { // For Non Imm Versions or Imm Versions like LDWR or MOVI
            switch (instructions[lineTokens[0]].index) {
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
                    std::string immParamToken = lineTokens[2];

                    if (immParamToken[0] == '0' && immParamToken[1] == 'x') {
                        return 4;
                    }
                    else if (immParamToken[0] == '0' && immParamToken[1] == 'b') {
                        return 4;
                    }
                    else if (!IsStrAlphaA(immParamToken)) {
                        auto it = instrParamConv.find(immParamToken);

                        if (it == instrParamConv.end()) { // Does Not Contain Key
                            return 4;
                        }
                    }

                    break;
                }
                default: {
                    std::cout << "Error: Less Tokens than Required for Instruction: " << lineTokens[0] << ", " << line << "\n";
                    return 0;
                }
            }
        }

        return instructions[lineTokens[0]].byteSize;
    }
    else {
        std::cout << "Error: Invalid Instruction Opcode: " << lineTokens[0] << ", " << line << "\n";
        return 0;
    }
}

void SetLabels(const std::vector<std::string>& lines) {
    for (int i = 0; i < lines.size(); i++) {
        currentAddress += CheckLine(lines[i]);
    }
}

int ConvertLineInstruction(std::string line) {
    std::vector<std::string> lineTokens;

    std::string str;

    std::cout << line << "\n";

    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ' ') {
            lineTokens.push_back(str);
            str = "";
        }
        else if (line[i] == '[') {
            str = "";
        }
        else if (line[i] == ']') {
            lineTokens.push_back(str);
            str = "";
            break;
        }
        else {
            str.push_back(line[i]);

            if (i >= line.size() - 1) {
                lineTokens.push_back(str);
                break;
            }
        }
    }

    if (lineTokens[0][0] == '%') {
        if (lineTokens[0].substr(1) == "org") {
            if (lineTokens.size() > 1) {
                currentAddress = ConvertInstrParam(lineTokens[1]);
            }
        }
        else if (lineTokens[0].substr(1) == "dw") {
            while (program.size() <= currentAddress) {
                program.push_back(0);
            }
            if (lineTokens.size() > 1) {
                uint16_t instrParam = ConvertInstrParam(lineTokens[1]);
                program[currentAddress] = (instrParam & 0xff00) >> 8;
                program.push_back(instrParam & 0x00ff);
            }
            else {
                program[currentAddress] = 0;
                program.push_back(0);
            }
            return 2;
        }
        else if (lineTokens[0].substr(1) == "db") {
            while (program.size() <= currentAddress) {
                program.push_back(0);
            }
            if (lineTokens.size() > 1) {
                program[currentAddress] = ConvertInstrParam(lineTokens[1]);
            }
            else {
                program[currentAddress] = 0;
            }
            return 1;
        }
        return 0;
    }
    else if (lineTokens[0][0] == '/' && lineTokens[0][1] == '/') {
        return 0;
    }

    std::string instrToken = lineTokens[0];

    uint8_t instr;
    auto it = instructions.find(instrToken);

    if (it != instructions.end()) { // Contains Key
        instr = instructions[instrToken].index;
    }
    else {
        std::cout << "Error: Invalid Instruction Opcode: " << instrToken << ", " << line << "\n";
        return 0;
    }
    uint16_t instrParam = 0;

    uint16_t extraParam = 0;
    bool extraParamOn = false;

    if (lineTokens.size() - 1 != instructions[instrToken].parameters.size() || instr == MOV) { // For Non Imm Versions or Imm Versions like LDWR or MOVI
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
                std::string immParamToken = lineTokens[2];

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
                else if (!IsStrAlphaA(immParamToken)) {
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
                std::cout << "Error: Less Tokens than Required for Instruction: " << lineTokens[0] << ", " << line << "\n";
                return 0;
            }
        }
    }

    for (int i = 0; i < instructions[instrToken].parameters.size(); i++) {
        if (instructions[instrToken].parameters[i] == ParameterIndex::OP0) {
            instrParam |= ((ConvertInstrParam(lineTokens[i + 1]) & 0b11) - 1 << 8);
        }
        else if (instructions[instrToken].parameters[i] == ParameterIndex::OP1) {
            instrParam |= ((ConvertInstrParam(lineTokens[i + 1]) & 0b1111) << 4);
        }
        else if (instructions[instrToken].parameters[i] == ParameterIndex::OP2) {
            instrParam |= (ConvertInstrParam(lineTokens[i + 1]) & 0b1111);
        }
        else if (instructions[instrToken].parameters[i] == ParameterIndex::IMM) {
            extraParam = ConvertInstrParam(lineTokens[i + 1]);
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

void CreateProgram(const std::vector<std::string>& lines) {
    currentAddress = 0;

    for (int i = 0; i < lines.size(); i++) {
        if (lines[i][0] == ':') {
            continue;
        }
        currentAddress += ConvertLineInstruction(lines[i]);
    }
}

void PrintProgram() {
    std::cout << "---" << "\n";

    int skipped = 0;

    for (int i = 0; i < program.size(); i += 2) {
        if (i >= 2) {
            if (program[i - 2] == 0 && program[i - 1] == 0 && program[i] == 0 && program[i + 1] == 0) {
                skipped++;
                continue;
            }
        }
        if (skipped != 0) {
            std::cout << "* - " << skipped * 2 << "\n";
            skipped = 0;
        }

        std::cout << i << ": " << std::bitset<8>(program[i]) << " - " << i + 1 << ": " << std::bitset<8>(program[i + 1]) << "\n";
    }
    std::cout << "Bytes: " << program.size() << "\n" ;
}

void Preprosessor(std::vector<std::string>& lines) {
    std::unordered_map<std::string, std::vector<std::string>> defines;

    for (int i = 0; i < lines.size(); i++) {
        std::vector<std::string> lineTokens;
        std::vector<std::string> instructions;
        std::string str;

        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == ' ' && lineTokens.size() <= 1) {
                lineTokens.push_back(str);
                str = "";
            }
            else {
                str.push_back(lines[i][j]);

                if (j >= lines[i].size() - 1) {
                    lineTokens.push_back(str);
                    break;
                }
            }
        }

        if (lineTokens[0] == "%define") {
            instructions = SplitA(lineTokens[2], '&');

            defines[lineTokens[1]] = instructions;
        }
        else if (lineTokens[0] == "%include") {
            std::string includePath = lines[i].substr(lines[i].find(' ') + 2, lines[i].substr(lines[i].find(' ') + 2).size() - 1);
            std::ifstream includeFile(includePath);

            if (!includeFile.is_open()) {
                std::cout << "Unable to open include file: " << includePath << "\n";
                includeFile.close();
            }
            
            std::string line;
        
            int offset = 1;
            while(getline(includeFile, line)) {
                line = TrimA(line);

                if (line.empty()) {
                    continue;
                }

                std::cout << line << "\n";
                
                lines.insert(lines.begin() + i + offset, line);
                offset++;
            }
            includeFile.close();
        }

        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == '$') {
                int defineS = j + 1, defineE = lines[i].substr(j + 1).find(' ');

                if (defineE == -1) {
                    defineE = lines[i].size() - 1;
                }

                std::vector<std::string> define = defines[lines[i].substr(defineS, defineE)];

                lines[i].replace(defineS, defineE - defineS + 1, defines[lines[i].substr(defineS, defineE)][0]);
                lines[i].erase(defineS - 1, 1);

                for (int k = 1; k < define.size(); k++) {
                    std::cout << define[k] << "\n";

                    lines.insert(lines.begin() + i + 1, define[k]);
                }
            }
        }
    }
    SetLabels(lines);
}

std::vector<char> AssembleLines(std::vector<std::string>& lines) {
    Preprosessor(lines);
    CreateProgram(lines);

    PrintProgram();

    return program;
}
