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

enum InstrCode {
    NOP = 0, MOV, LI,
    LDW, LDB, STW, STB, LDE, STE,
    JMP, JMPF, CMP, JZ, JC,
    ADD, SUB, INC, DEC, MULT, DIV, AND, OR,
    PUSH, POP, CALL, CALLF, RET, RETF
};
enum ParameterIndex {
    RS = 0, RS1, RS2, IMM
};

struct Instruction {
    int index;
    int byteSize;
    std::vector<ParameterIndex> parameters;
};

std::unordered_map<std::string, Instruction> instructions {
    { "nop",   { NOP,   2, {} } },
    { "mov",   { MOV,   2, { RS1, RS2 } } },
    { "li",    { LI,    4, { RS1, IMM } } },

    { "ldw",   { LDW,   4, { RS, RS1, RS2, IMM } } },
    { "ldb",   { LDB,   4, { RS, RS1, RS2, IMM } } },
    { "stw",   { STW,   4, { RS, RS1, RS2, IMM } } },
    { "stb",   { STB,   4, { RS, RS1, RS2, IMM } } },
    { "lde",   { LDE,   4, { RS, RS1, RS2, IMM } } },
    { "ste",   { STE,   4, { RS, RS1, RS2, IMM } } },

    { "jmp",   { JMP,   4, { RS2, IMM } } },
    { "jmpf",  { JMPF,  4, { RS1, RS2, IMM } } },
    { "cmp",   { CMP,   2, { RS1, RS2 } } },
    { "jz",    { JZ,    4, { RS2, IMM } } },
    { "jc",    { JC,    4, { RS2, IMM } } },

    { "add",   { ADD,   2, { RS, RS1, RS2 } } },
    { "sub",   { SUB,   2, { RS, RS1, RS2 } } },
    { "inc",   { INC,   2, { RS, RS1 } } },
    { "dec",   { DEC,   2, { RS, RS1 } } },
    { "mult",  { MULT,  2, { RS, RS1, RS2 } } },
    { "div",   { DIV,   2, { RS, RS1, RS2 } } },
    { "and",   { AND,   2, { RS, RS1, RS2 } } },
    { "or",    { OR,    2, { RS, RS1, RS2 } } },

    { "push",  { PUSH,  2, { RS1 } } },
    { "pop",   { POP,   2, { RS1 } } },
    { "call",  { CALL,  4, { RS2, IMM } } },
    { "callf", { CALLF, 4, { RS1, RS2, IMM } } },
    { "ret",   { RET,   2, {} } },
    { "retf",  { RETF,  2, {} } },
};

// Register Index
#define R1  1
#define R2  2
#define R3  3
#define R4  4

#define SP  5
#define BP  6

#define CS  7
#define DS  8
#define SS  9
#define ES  10

#define Z   11

// Segment Indexes
#define CSS  1
#define DSS  2
#define SSS  3
#define ESS  4

// Ext Port Indexes
#define E1  1
#define E2  2
#define E3  3
#define E4  4

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
        if      (instrParamStr == "c") { instrParam = CSS; }
        else if (instrParamStr == "d") { instrParam = DSS; }
        else if (instrParamStr == "s") { instrParam = SSS; }
        else if (instrParamStr == "e") { instrParam = ESS; }

        else if (instrParamStr == "r1") { instrParam = R1; }
        else if (instrParamStr == "r2") { instrParam = R2; }
        else if (instrParamStr == "r3") { instrParam = R3; }
        else if (instrParamStr == "r4") { instrParam = R4; }

        else if (instrParamStr == "sp") { instrParam = SP; }
        else if (instrParamStr == "bp") { instrParam = BP; }

        else if (instrParamStr == "cs")  { instrParam = CS; }
        else if (instrParamStr == "ds")  { instrParam = DS; }
        else if (instrParamStr == "ss")  { instrParam = SS; }
        else if (instrParamStr == "es")  { instrParam = ES; }

        else if (instrParamStr == "z")   { instrParam = Z; }

        else if (instrParamStr == "e1")  { instrParam = E1; }
        else if (instrParamStr == "e2")  { instrParam = E2; }
        else if (instrParamStr == "e3")  { instrParam = E3; }
        else if (instrParamStr == "e4")  { instrParam = E4; }

        else instrParam = labels[instrParamStr];
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

    return instructions[lineTokens[0]].byteSize;
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

    uint8_t instr = instructions[lineTokens[0]].index;
    uint16_t instrParam = 0;

    uint16_t extraParam = 0;
    bool extraParamOn = false;

    for (int i = 0; i < instructions[lineTokens[0]].parameters.size(); i++) {
        if (instructions[lineTokens[0]].parameters[i] == ParameterIndex::RS) {
            instrParam |= ((ConvertInstrParam(lineTokens[i + 1]) & 0b11) - 1 << 8);
        }
        else if (instructions[lineTokens[0]].parameters[i] == ParameterIndex::RS1) {
            instrParam |= ((ConvertInstrParam(lineTokens[i + 1]) & 0b1111) << 4);
        }
        else if (instructions[lineTokens[0]].parameters[i] == ParameterIndex::RS2) {
            instrParam |= (ConvertInstrParam(lineTokens[i + 1]) & 0b1111);
        }
        else if (instructions[lineTokens[0]].parameters[i] == ParameterIndex::IMM) {
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
