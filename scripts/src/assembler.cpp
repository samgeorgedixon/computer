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

std::vector<uint16_t> program;
std::unordered_map<std::string, unsigned int> labels;
int currentAddress = 0;

enum InstrCode {
    NOP = 0, MOV, LI,
    LDW, STW, LDEW, STEW,
    JMP, JMPF, CMP, JZ, JC,
    ADD, SUB, INC, DEC, MULT, DIV, AND, OR,
    PUSH, POP, CALL, CALLF, RET, RETF
};
enum ParameterIndex {
    RS = 0, RS1, RS2, IMM
};

struct Instruction {
    int index;
    int wordSize;
    std::vector<ParameterIndex> parameters;
};

std::unordered_map<std::string, Instruction> instructions {
    { "nop",   { NOP,   1, {} } },
    { "mov",   { MOV,   1, { RS1, RS2 } } },
    { "li",    { LI,    2, { RS1, IMM } } },

    { "ldw",   { LDW,   2, { RS, RS1, RS2, IMM } } },
    { "stw",   { STW,   2, { RS, RS1, RS2, IMM } } },
    { "ldew",   { LDEW,  2, { RS, RS1, RS2, IMM } } },
    { "stew",   { STEW,  2, { RS, RS1, RS2, IMM } } },

    { "jmp",   { JMP,   2, { RS2, IMM } } },
    { "jmpf",  { JMPF,  2, { RS1, RS2, IMM } } },
    { "cmp",   { CMP,   1, { RS1, RS2 } } },
    { "jz",    { JZ,    2, { RS2, IMM } } },
    { "jc",    { JC,    2, { RS2, IMM } } },

    { "add",   { ADD,   1, { RS, RS1, RS2 } } },
    { "sub",   { SUB,   1, { RS, RS1, RS2 } } },
    { "inc",   { INC,   1, { RS, RS1 } } },
    { "dec",   { DEC,   1, { RS, RS1 } } },
    { "mult",  { MULT,  1, { RS, RS1, RS2 } } },
    { "div",   { DIV,   1, { RS, RS1, RS2 } } },
    { "and",   { AND,   1, { RS, RS1, RS2 } } },
    { "or",    { OR,    1, { RS, RS1, RS2 } } },

    { "push",  { PUSH,  1, { RS1 } } },
    { "pop",   { POP,   1, { RS1 } } },
    { "call",  { CALL,  2, { RS2, IMM } } },
    { "callf", { CALLF, 2, { RS1, RS2, IMM } } },
    { "ret",   { RET,   1, {} } },
    { "retf",  { RETF,  1, {} } },
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
            return 1;
        } else if (lineTokens[0].substr(1) == "segment") {
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

    return instructions[lineTokens[0]].wordSize;
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
            std::cout << " : " << str << "\n";
            str = "";
        }
        else if (line[i] == '[') {
            str = "";
        }
        else if (line[i] == ']') {
            lineTokens.push_back(str);
            std::cout << "[: " << str << "\n";
            break;
        }
        else {
            str.push_back(line[i]);

            if (i >= line.size() - 1) {
                lineTokens.push_back(str);
                std::cout << "e: " << str << "\n";
                break;
            }
        }
    }
    std::cout << "-" << "\n";

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
                program[currentAddress] = ConvertInstrParam(lineTokens[1]).instrParam;
            }
            else program[currentAddress] = 0;

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

    program[currentAddress] = 0xFC00&(instr << 10)|0x03FF&(instrParam);

    if (extraParamOn) {
        program.push_back(extraParam);
        return 2;
    }

    return 1;
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

    for (int i = 0; i < program.size(); i++) {
        if (program[i - 1] == 0 && program[i] == 0) {
            skipped++;
        } else {
            if (skipped != 0) {
                std::cout << "* - " << skipped << "\n";
                skipped = 0;
            }
            std::cout << "Instr: " << std::bitset<6>(program[i] >> 10)
                 << " - Param: " << std::bitset<10>(program[i]) << "\n";
        }
    }

    std::cout << "Words: " << program.size() << "\n" ;
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

    for (int i = 0; i < lines.size(); i++) {
        std::cout << lines[i] << "\n";
    }

    SetLabels(lines);

    CreateProgram(lines);

    PrintProgram();

    std::ofstream outFile(outPath, std::ios::out | std::ios::binary);

    std::vector<char> programBytes;
    for (int i = 0; i < program.size(); i++) {
        for (int j = 1; j >= 0; j--) {
            programBytes.push_back(program[i] >> (j * 8));
        }
    }
    outFile.write(&programBytes[0], programBytes.size() * sizeof(char));
    outFile.close();
    
    return 0;
}
