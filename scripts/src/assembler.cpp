#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cctype>
#include <bitset>

std::string srcPath;
std::string outPath;

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

std::string Trim(std::string str, std::string whitespace = " \t\r") {
    int strBegin = str.find_first_not_of(whitespace);

    if (strBegin == std::string::npos) {
        return "";
    }

    int strEnd = str.find_last_not_of(whitespace);
    int strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void GetPaths(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'o') {
            i++;
            outPath = argv[i];
        } else {
            srcPath = argv[i];
        }
    }
}

bool IsStrAlpha(std::string str) {
    bool strAlpha = false;
    for (int i = 0; i < str.length(); i++) {
        strAlpha = isalpha(str[i]);
        if (strAlpha)
            return strAlpha;
    }
    return strAlpha;
}

struct InstrParamConverted {
    int instrParam;
    bool isSeg;
    bool isExt;
    bool isReg;
};

InstrParamConverted ConvertInstrParam(std::string instrParamStr) {
    int instrParam = 0;
    bool isSeg = false;
    bool isExt = false;
    bool isReg = false;

    if (instrParamStr[0] == '0' && instrParamStr[1] == 'x') {
        instrParam = std::stoi(instrParamStr.substr(2), nullptr, 16);
    }
    else if (instrParamStr[0] == '0' && instrParamStr[1] == 'b') {
        instrParam = std::stoi(instrParamStr.substr(2), nullptr, 2);
    }
    else if (IsStrAlpha(instrParamStr)) {
        if      (instrParamStr == "c") { instrParam = CSS; isSeg = true; }
        else if (instrParamStr == "d") { instrParam = DSS; isSeg = true; }
        else if (instrParamStr == "s") { instrParam = SSS; isSeg = true; }
        else if (instrParamStr == "e") { instrParam = ESS; isSeg = true; }

        else if (instrParamStr == "r1") { instrParam = R1; isReg = true; }
        else if (instrParamStr == "r2") { instrParam = R2; isReg = true; }
        else if (instrParamStr == "r3") { instrParam = R3; isReg = true; }
        else if (instrParamStr == "r4") { instrParam = R4; isReg = true; }

        else if (instrParamStr == "sp") { instrParam = SP; isReg = true; }
        else if (instrParamStr == "bp") { instrParam = BP; isReg = true; }

        else if (instrParamStr == "cs")  { instrParam = CS; isReg = true; }
        else if (instrParamStr == "ds")  { instrParam = DS; isReg = true; }
        else if (instrParamStr == "ss")  { instrParam = SS; isReg = true; }
        else if (instrParamStr == "es")  { instrParam = ES; isReg = true; }

        else if (instrParamStr == "z")   { instrParam = Z; isReg = true; }

        else if (instrParamStr == "e1")  { instrParam = E1; isExt = true; }
        else if (instrParamStr == "e2")  { instrParam = E2; isExt = true; }
        else if (instrParamStr == "e3")  { instrParam = E3; isExt = true; }
        else if (instrParamStr == "e4")  { instrParam = E4; isExt = true; }

        else instrParam = labels[instrParamStr];
    }
    else {
        instrParam = std::stoi(instrParamStr);
    }

    return { instrParam, isSeg, isExt, isReg };
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
                currentAddress = ConvertInstrParam(lineTokens[1]).instrParam;
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

void SetLabels(std::vector<std::string> lines) {
    for (int i = 0; i < lines.size(); i++) {
        currentAddress += CheckLine(lines[i]);
    }
}

int ConvertLineInstruction(std::string line) {
    std::vector<std::string> lineTokens;

    std::string str;

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
                currentAddress = ConvertInstrParam(lineTokens[1]).instrParam;
            }
        }
        else if (lineTokens[0].substr(1) == "dw") {
            while (program.size() <= currentAddress) {
                program.push_back(0);
            }
            if (lineTokens.size() > 1) {
                uint16_t instrParam = ConvertInstrParam(lineTokens[1]).instrParam;
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
                program[currentAddress] = ConvertInstrParam(lineTokens[1]).instrParam;
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
            instrParam |= ((ConvertInstrParam(lineTokens[i + 1]).instrParam & 0b11) - 1 << 8);
        }
        else if (instructions[lineTokens[0]].parameters[i] == ParameterIndex::RS1) {
            instrParam |= ((ConvertInstrParam(lineTokens[i + 1]).instrParam & 0b1111) << 4);
        }
        else if (instructions[lineTokens[0]].parameters[i] == ParameterIndex::RS2) {
            instrParam |= (ConvertInstrParam(lineTokens[i + 1]).instrParam & 0b1111);
        }
        else if (instructions[lineTokens[0]].parameters[i] == ParameterIndex::IMM) {
            extraParam = ConvertInstrParam(lineTokens[i + 1]).instrParam;
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

void CreateProgram(std::vector<std::string> lines) {
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

int main(int argc, char* argv[]) {
    GetPaths(argc, argv);

    std::ifstream srcFile(srcPath);

    if (!srcFile.is_open()) {
        std::cout << "Unable to open src file.\n";

        srcFile.close();
        return 1;
    }
    
    std::string line;
    std::vector<std::string> lines;

    while(getline(srcFile, line)) {
        line = Trim(line);

        if (line.empty()) {
            continue;
        }
        
        lines.push_back(line);
    }
    srcFile.close();

    SetLabels(lines);

    CreateProgram(lines);

    PrintProgram();

    std::ofstream outFile(outPath, std::ios::out | std::ios::binary);
    outFile.write(&program[0], program.size() * sizeof(char));
    outFile.close();
    
    return 0;
}
