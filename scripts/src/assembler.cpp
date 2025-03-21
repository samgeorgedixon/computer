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
    NOP   = 0b000000, MOV   = 0b000001, LDR   = 0b000011, STR   = 0b000111,
    LDEPA = 0b001011, STEPA = 0b001101, LDEPB = 0b001111, STEPB = 0b010001,
    LDEPC = 0b010011, STEPC = 0b010101, LDEPD = 0b010111, STEPD = 0b011001,
    JMP   = 0b011011, JMPF  = 0b011101, CMP   = 0b011111, JZ    = 0b100000, JC   = 0b100010,
    ADD   = 0b100100, SUB   = 0b100101, INC   = 0b100110, DEC   = 0b100111, MULT = 0b101000, DIV  = 0b101001, AND = 0b101010, OR = 0b101011,
    PUSH  = 0b101100, POP   = 0b101101, CALL  = 0b101110, CALLF = 0b110000, RET  = 0b110010, RETF = 0b110011
};

std::unordered_map<std::string, uint8_t> instructions {
    { "nop",   NOP },
    { "mov",   MOV },

    { "ldr",   LDR },



    { "str",   STR },



    { "ldepa", LDEPA },

    { "stepa", STEPA },

    { "ldepb", LDEPB },

    { "stepb", STEPB },
    
    { "ldepc", LDEPC },

    { "stepc", STEPC },

    { "ldepd", LDEPD },

    { "stepd", STEPD },


    { "jmp",   JMP },

    { "jmpf",  JMPF },

    { "cmp",   CMP },
    { "jz",    JZ },

    { "jc",    JC },
    

    { "add",   ADD },
    { "sub",   SUB },
    { "inc",   INC },
    { "dec",   DEC },
    { "mult",  MULT },
    { "div",   DIV },
    { "and",   AND },
    { "or",    OR },

    { "push",  PUSH },
    { "pop",   POP },
    { "call",  CALL },

    { "callf", CALLF },

    { "ret",   RET },
    { "retf",  RETF }
};

// Register Index
#define RA  1
#define RB  2
#define RC  3
#define RD  4
#define RE  5

#define SP  6
#define BP  7

#define CS  8
#define DS  9
#define SS  10
#define ES  11

// Segment Index
#define CSS  0
#define DSS  1
#define SSS  2
#define ESS  3

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
    bool isReg;
};

InstrParamConverted ConvertInstrParam(std::string instrParamStr) {
    int instrParam = 0;
    bool isSeg = false;
    bool isReg = false;
    if (instrParamStr[0] == '0' && instrParamStr[1] == 'x') {
        instrParam = std::stoi(instrParamStr.substr(2), nullptr, 16);
    } else if (instrParamStr[0] == '0' && instrParamStr[1] == 'b') {
        instrParam = std::stoi(instrParamStr.substr(2), nullptr, 2);
    } else if (IsStrAlpha(instrParamStr)) {
        if      (instrParamStr == "c") { instrParam = CSS; isSeg = true; }
        else if (instrParamStr == "d") { instrParam = DSS; isSeg = true; }
        else if (instrParamStr == "s") { instrParam = SSS; isSeg = true; }
        else if (instrParamStr == "e") { instrParam = ESS; isSeg = true; }

        else if (instrParamStr == "ra") { instrParam = RA; isReg = true; }
        else if (instrParamStr == "rb") { instrParam = RB; isReg = true; }
        else if (instrParamStr == "rc") { instrParam = RC; isReg = true; }
        else if (instrParamStr == "rd") { instrParam = RD; isReg = true; }
        else if (instrParamStr == "re") { instrParam = RE; isReg = true; }

        else if (instrParamStr == "sp") { instrParam = SP; isReg = true; }
        else if (instrParamStr == "bp") { instrParam = BP; isReg = true; }

        else if (instrParamStr == "cs")  { instrParam = CS; isReg = true; }
        else if (instrParamStr == "ds")  { instrParam = DS; isReg = true; }
        else if (instrParamStr == "ss")  { instrParam = SS; isReg = true; }
        else if (instrParamStr == "es")  { instrParam = ES; isReg = true; }
        else instrParam = labels[instrParamStr];
    } else {
        instrParam = std::stoi(instrParamStr);
    }
    return { instrParam, isSeg, isReg };
}

int ProgramOutLine(std::string line) {
    std::string str;
    std::stringstream ss(line);
    std::vector<std::string> lineTokens;

    while (getline(ss, str, ' '))
        lineTokens.push_back(str);

    if (lineTokens[0][0] == '%') {
        if (lineTokens[0].substr(1) == "org") {
            if (lineTokens.size() > 1)
                currentAddress = ConvertInstrParam(lineTokens[1]).instrParam;
        } else if (lineTokens[0].substr(1) == "dw") {
            while (program.size() <= currentAddress)
                program.push_back(0);
            if (lineTokens.size() > 1)
                program[currentAddress] = ConvertInstrParam(lineTokens[1]).instrParam;
            else program[currentAddress] = 0;
            return 1;
        }
        return 0;
    }
    else if (lineTokens[0][0] == '/' && lineTokens[0][1] == '/')
        return 0;

    uint8_t instr = instructions[lineTokens[0]];
    uint16_t instrParam = 0;

    uint16_t extraParam = 0;
    bool extraParamOn = false;

    switch (instructions[lineTokens[0]]) {
    case InstrCode::NOP: {
        break;
    }
    case InstrCode::MOV: {
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[1]);
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[2]);
        instrParam = ((destReg.instrParam & 0b1111) << 4);

        if (!srcReg.isReg) {
            extraParam = srcReg.instrParam; instr++; extraParamOn = true;
        } else
            instrParam = instrParam|(srcReg.instrParam & 0b1111);
        break;
    }
    case InstrCode::LDR: {
        InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
        int i = 1;
        instr += 2;
        if (seg.isSeg) {
            instrParam = ((seg.instrParam & 0b11) << 8);
            i++; instr -= 2;
        }
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]); i++;
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
        instrParam |= ((destReg.instrParam & 0b1111) << 4);

        if (!srcReg.isReg) {
            extraParam = srcReg.instrParam; instr++; extraParamOn = true;
        } else
            instrParam = instrParam|(srcReg.instrParam & 0b1111);
        break;
    }
    case InstrCode::STR: {
        InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
        int i = 1;
        instr += 2;
        if (seg.isSeg) {
            instrParam = ((seg.instrParam & 0b11) << 8);
            i++; instr -= 2;
        }
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]); i++;
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
        instrParam = (srcReg.instrParam & 0b1111);
        
        if (!srcReg.isReg) {
            extraParam = destReg.instrParam; instr++; extraParamOn = true;
        } else
            instrParam = instrParam|((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::LDEPA: {
        InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
        int i = 1;
        instr++;
        if (seg.isSeg) {
            instrParam = ((seg.instrParam & 0b11) << 8);
            i++; instr--;
        }
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]); i++;
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
        instrParam |= ((destReg.instrParam & 0b1111) << 4);
        instrParam |= (srcReg.instrParam & 0b1111);
        break;
    }
    case InstrCode::STEPA: {
        InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
        int i = 1;
        instr++;
        if (seg.isSeg) {
            instrParam = ((seg.instrParam & 0b11) << 8);
            i++; instr--;
        }
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]); i++;
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
        instrParam = (srcReg.instrParam & 0b1111);
        instrParam = instrParam|((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::LDEPB: {
        InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
        int i = 1;
        instr++;
        if (seg.isSeg) {
            instrParam = ((seg.instrParam & 0b11) << 8);
            i++; instr--;
        }
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]); i++;
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
        instrParam |= ((destReg.instrParam & 0b1111) << 4);
        instrParam |= (srcReg.instrParam & 0b1111);
        break;
    }
    case InstrCode::STEPB: {
        InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
        int i = 1;
        instr++;
        if (seg.isSeg) {
            instrParam = ((seg.instrParam & 0b11) << 8);
            i++; instr--;
        }
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]); i++;
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
        instrParam = (srcReg.instrParam & 0b1111);
        instrParam = instrParam|((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::LDEPC: {
        InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
        int i = 1;
        instr++;
        if (seg.isSeg) {
            instrParam = ((seg.instrParam & 0b11) << 8);
            i++; instr--;
        }
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]); i++;
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
        instrParam |= ((destReg.instrParam & 0b1111) << 4);
        instrParam |= (srcReg.instrParam & 0b1111);
        break;
    }
    case InstrCode::STEPC: {
        InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
        int i = 1;
        instr++;
        if (seg.isSeg) {
            instrParam = ((seg.instrParam & 0b11) << 8);
            i++; instr--;
        }
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]); i++;
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
        instrParam = (srcReg.instrParam & 0b1111);
        instrParam = instrParam|((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::LDEPD: {
        InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
        int i = 1;
        instr++;
        if (seg.isSeg) {
            instrParam = ((seg.instrParam & 0b11) << 8);
            i++; instr--;
        }
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]); i++;
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
        instrParam |= ((destReg.instrParam & 0b1111) << 4);
        instrParam |= (srcReg.instrParam & 0b1111);
        break;
    }
    case InstrCode::STEPD: {
        InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
        int i = 1;
        instr++;
        if (seg.isSeg) {
            instrParam = ((seg.instrParam & 0b11) << 8);
            i++; instr--;
        }
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]); i++;
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
        instrParam = (srcReg.instrParam & 0b1111);
        instrParam = instrParam|((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::JMP: {
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[1]);
        if (!srcReg.isReg) {
            extraParam = srcReg.instrParam; instr++; extraParamOn = true;
        } else
            instrParam = instrParam|(srcReg.instrParam & 0b1111);
        break;
    }
    case InstrCode::CMP: {
        break;
    }
    case InstrCode::JZ: {
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[1]);
        if (!srcReg.isReg) {
            extraParam = srcReg.instrParam; instr++; extraParamOn = true;
        } else
            instrParam = instrParam|(srcReg.instrParam & 0b1111);
        break;
    }
    case InstrCode::JC: {
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[1]);
        if (!srcReg.isReg) {
            extraParam = srcReg.instrParam; instr++; extraParamOn = true;
        } else
            instrParam = instrParam|(srcReg.instrParam & 0b1111);
        break;
    }
    case InstrCode::ADD: {
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[1]);
        instrParam = ((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::SUB: {
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[1]);
        instrParam = ((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::INC: {
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[1]);
        instrParam = ((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::DEC: {
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[1]);
        instrParam = ((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::MULT: {
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[1]);
        instrParam = ((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::DIV: {
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[1]);
        instrParam = ((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::AND: {
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[1]);
        instrParam = ((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::OR: {
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[1]);
        instrParam = ((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::PUSH: {
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[1]);
        instrParam = (srcReg.instrParam & 0b1111);
        break;
    }
    case InstrCode::POP: {
        InstrParamConverted destReg = ConvertInstrParam(lineTokens[1]);
        instrParam = ((destReg.instrParam & 0b1111) << 4);
        break;
    }
    case InstrCode::CALL: {
        InstrParamConverted srcReg = ConvertInstrParam(lineTokens[1]);
        if (!srcReg.isReg) {
            extraParam = srcReg.instrParam; instr++; extraParamOn = true;
        } else
            instrParam = instrParam|(srcReg.instrParam & 0b1111);
        break;
    }
    case InstrCode::RET: {
        break;
    }
    }

    while (program.size() <= currentAddress)
        program.push_back(0);

    program[currentAddress] = 0xFC00&(instr << 10)|0x03FF&(instrParam);

    if (extraParamOn) {
        program.push_back(extraParam);
        return 2;
    }
    return 1;
}

int LexLine(std::string line) {
    std::string str;
    std::stringstream ss(line);
    std::vector<std::string> lineTokens;
    while (getline(ss, str, ' '))
       lineTokens.push_back(str);
    if (line[0] == '%') {
        if (lineTokens[0].substr(1) == "org") {
            if (lineTokens.size() > 1)
                currentAddress = ConvertInstrParam(lineTokens[1]).instrParam;
        } else if (lineTokens[0].substr(1) == "dw") {
            return 1;
        } else if (lineTokens[0].substr(1) == "segment") {
            currentAddress = 0;
        }
        return 0;
    }
    if (line[0] == ':') {
        labels[line.substr(line.find(":") + 1)] = currentAddress;
        return 0;
    }
    if (line[0] == '/' && line[1] == '/')
        return 0;
    if (lineTokens[0] == "mov" || lineTokens[0] == "ldr" || lineTokens[0] == "str" ||
    lineTokens[0] == "jmp" || lineTokens[0] == "jz" || lineTokens[0] == "jc" || lineTokens[0] == "call") {
        bool immediate = false;
        switch (instructions[lineTokens[0]]) {
        case InstrCode::MOV: {
            InstrParamConverted destReg = ConvertInstrParam(lineTokens[1]);
            InstrParamConverted srcReg = ConvertInstrParam(lineTokens[2]);
            if (!srcReg.isReg)
                immediate = true;
            break;
        }
        case InstrCode::LDR: {
            InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
            int i = 1;
            if (seg.isSeg)
                i++;
            InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]);
            InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
            if (!srcReg.isReg)
                immediate = true;
            break;
        }
        case InstrCode::STR: {
            InstrParamConverted seg = ConvertInstrParam(lineTokens[1]);
            int i = 1;
            if (seg.isSeg)
                i++;
            InstrParamConverted destReg = ConvertInstrParam(lineTokens[i]);
            InstrParamConverted srcReg = ConvertInstrParam(lineTokens[i]);
            if (!srcReg.isReg)
                immediate = true;
            break;
        }
        case InstrCode::JMP: {
            InstrParamConverted srcReg = ConvertInstrParam(lineTokens[1]);
            if (!srcReg.isReg)
                immediate = true;
            break;
        }
        case InstrCode::JZ: {
            InstrParamConverted srcReg = ConvertInstrParam(lineTokens[1]);
            if (!srcReg.isReg)
                immediate = true;
            break;
        }
        case InstrCode::JC: {
            InstrParamConverted srcReg = ConvertInstrParam(lineTokens[1]);
            if (!srcReg.isReg)
                immediate = true;
            break;
        }
        case InstrCode::CALL: {
            InstrParamConverted srcReg = ConvertInstrParam(lineTokens[1]);
            if (!srcReg.isReg)
                immediate = true;
            break;
        }
        }
        if (immediate)
            return 2;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    GetPaths(argc, argv);

    std::ifstream srcFile(srcPath);

    if (!srcFile.is_open()) {
        std::cout << "Unable to open src file.\n";
        srcFile.close(); return 1;
    }
    std::string line;

    // Lexer
    while(getline(srcFile, line)) {
        if (line == "")
            continue;
        currentAddress += LexLine(line);
    }
    srcFile.clear(); srcFile.seekg(0);
    // Create Program
    currentAddress = 0;
    while(getline(srcFile, line)) {
        if (line == "" || line[0] == ':')
            continue;
        currentAddress += ProgramOutLine(line);
    }
    srcFile.close();

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
