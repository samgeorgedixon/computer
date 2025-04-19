#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cctype>
#include <bitset>
#include <algorithm>
#include <iterator>

#include "assembler.h"

std::string Trim(std::string str, std::string whitespace = " \t\r") {
    int strBegin = str.find_first_not_of(whitespace);

    if (strBegin == std::string::npos) {
        return "";
    }

    int strEnd = str.find_last_not_of(whitespace);
    int strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::vector<std::string> Split(std::string str, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(Trim(item));
    }
    if (result.size() == 0 && Trim(str).size() != 0) {
        result.push_back(Trim(str));
    }

    return result;
}

struct Paths {
    std::string src;
    std::string out;
    std::string asmb;
};

Paths GetPaths(int argc, char* argv[]) {
    Paths paths = { "", "", "" };

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'o') {
            i++;
            paths.out = argv[i];
        }
        else if ((argv[i][0] == '-' && argv[i][1] == 'a')) {
            i++;
            paths.asmb = argv[i];
        }
        else {
            paths.src = argv[i];
        }
    }

    return paths;
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

std::vector<std::vector<std::string>> ParseFileLines(std::ifstream& srcFile) {
    std::vector<std::vector<std::string>> lines;

    std::string line;
    
    while(getline(srcFile, line)) {
        line = Trim(line);

        if (line.empty()) {
            continue;
        }

        if (line[0] == '#') {
            std::vector<std::string> lineTokens;
            int lastTokenEnd = 0;
            bool isString = false;

            for (int i = 0; i < line.size(); i++) {
                if (line[i] == ' ') {
                    if (isString) {
                        continue;
                    }
    
                    if (!Trim(line.substr(lastTokenEnd, i - lastTokenEnd)).empty()) {
                        lineTokens.push_back(Trim(line.substr(lastTokenEnd, i - lastTokenEnd)));
                    }
                    lastTokenEnd = i + 1;
                }
                else if (line[i] == '"') {
                    if (!isString) {
                        isString = true;
                    }
                    else {
                        isString = false;
                    }
    
                    if (!Trim(line.substr(lastTokenEnd, i - lastTokenEnd)).empty()) {
                        lineTokens.push_back(Trim(line.substr(lastTokenEnd, i - lastTokenEnd)));
                    }
                    lineTokens.push_back("\"");
                    lastTokenEnd = i + 1;
                }
            }

            lines.push_back(lineTokens);
        }

        std::vector<std::string> lineTokens;
        int lastTokenEnd = 0;
        bool isString = false;

        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ';') {
                if (isString) {
                    continue;
                }

                if (!Trim(line.substr(lastTokenEnd, i - lastTokenEnd)).empty()) {
                    lineTokens.push_back(Trim(line.substr(lastTokenEnd, i - lastTokenEnd)));
                }
                lastTokenEnd = i + 1;
                lines.push_back(lineTokens);
            }
            else if (line[i] == '{') {
                if (isString) {
                    continue;
                }

                if (!Trim(line.substr(lastTokenEnd, i - lastTokenEnd)).empty()) {
                    lineTokens.push_back(Trim(line.substr(lastTokenEnd, i - lastTokenEnd)));
                }
                lastTokenEnd = i + 1;
                lines.push_back(lineTokens);
            }
            else if (line[i] == '}') {
                if (isString) {
                    continue;
                }

                lines.push_back({ "}" });
                lastTokenEnd = i + 1;
            }
            else if (line[i] == ' ') {
                if (isString) {
                    continue;
                }

                if (!Trim(line.substr(lastTokenEnd, i - lastTokenEnd)).empty()) {
                    lineTokens.push_back(Trim(line.substr(lastTokenEnd, i - lastTokenEnd)));
                }
                lastTokenEnd = i + 1;
            }
            else if (line[i] == '(') {
                if (isString) {
                    continue;
                }

                if (!Trim(line.substr(lastTokenEnd, i - lastTokenEnd)).empty()) {
                    lineTokens.push_back(Trim(line.substr(lastTokenEnd, i - lastTokenEnd)));
                }
                lineTokens.push_back("(");
                lastTokenEnd = i + 1;
            }
            else if (line[i] == ')') {
                if (isString) {
                    continue;
                }

                if (!Trim(line.substr(lastTokenEnd, i - lastTokenEnd)).empty()) {
                    lineTokens.push_back(Trim(line.substr(lastTokenEnd, i - lastTokenEnd)));
                }
                lineTokens.push_back(")");
                lastTokenEnd = i + 1;
            }
            else if (line[i] == ',') {
                if (isString) {
                    continue;
                }

                if (!Trim(line.substr(lastTokenEnd, i - lastTokenEnd)).empty()) {
                    lineTokens.push_back(Trim(line.substr(lastTokenEnd, i - lastTokenEnd)));
                }
                lineTokens.push_back(",");
                lastTokenEnd = i + 1;
            }
            else if (line[i] == '"') {
                if (!isString) {
                    isString = true;
                }
                else {
                    isString = false;
                }

                if (!Trim(line.substr(lastTokenEnd, i - lastTokenEnd)).empty()) {
                    lineTokens.push_back(Trim(line.substr(lastTokenEnd, i - lastTokenEnd)));
                }
                lineTokens.push_back("\"");
                lastTokenEnd = i + 1;
            }
        }
    }
    return lines;
}

std::vector<std::vector<std::string>> LoadFile(std::string filePath) {
    std::vector<std::vector<std::string>> lines;

    std::ifstream srcFile(filePath);

    if (!srcFile.is_open()) {
        std::cout << "Unable to open src file.\n";

        srcFile.close();
        return {};
    }

    lines = ParseFileLines(srcFile);
    
    srcFile.close();

    return lines;
}

enum Types {
    N, VOID, INT16, INT8
};
std::unordered_map<std::string, Types> types = {
    { "",           N },
    { "void",       VOID },
    { "int16_t",    INT16 },
    { "int8_t",     INT8 },
    { "char",       INT8 },
    { "bool",       INT8 },
};

struct Params {
    bool isParams;
    std::vector<std::string> params;
};

struct VarData {
    int addr;
    bool isStack;
    Types type;
};

Params GetParams(const std::vector<std::string>& line) {
    bool isParams = false;
    std::vector<std::string> params;

    int missed = 0;

    for (int i = 0; i < line.size(); i++) {
        if (line[i] == "(") {
            isParams = true;
        }
        else if (line[i] == ")") {
            for (int j = missed; j > 0; j--) {
                params.push_back(line[i - j]);
            }
            missed = 0;
        }
        else if (line[i] == ",") {
            for (int j = missed; j > 0; j--) {
                params.push_back(line[i - j]);
            }
            missed = 0;
        }
        else if (line[i] == "\"") {
            i++;
            params.push_back(line[i]);
            i++;

            missed = 0;
        }
        else {
            if (isParams) {
                missed++;
            }
        }
    }

    if (!isParams) {
        params = {};
    }

    return { isParams, params };
}

struct TypeString {
    bool isFunction;
    bool isVariable;
    int value;
};

std::pair<bool, int> StringToType(std::string str) {
    if (str[0] == '0' && str[1] == 'x') {
        return { false, std::stoi(str.substr(2), nullptr, 16) };
    }
    else if (str[0] == '0' && str[1] == 'b') {
        return { false, std::stoi(str.substr(2), nullptr, 2) };
    }
    else if (str[0] == '\'') {
        return { false, str[1] };
    }
    else if (str == "true") {
        return { false, true };
    }
    else if (str == "false") {
        return { false, false };
    }
    else if (IsStrAlpha(str)) {
        return { true, 0 };
    }
    else {
        return { false, std::stoi(str) };
    }
    return { false, 0 };
}

void LdStVar(VarData& var, std::vector<std::string>& program, std::string reg, std::string ld_st) {
    if (var.isStack) {
        if      (var.type == Types::INT16) { program.push_back(ld_st + "w s " + reg + " [bp " + std::to_string(var.addr) + "]"); }
        else if (var.type == Types::INT8)  { program.push_back(ld_st + "b s " + reg + " [bp " + std::to_string(var.addr) + "]"); }
    }
    else {
        if      (var.type == Types::INT16) { program.push_back(ld_st + "w d " + reg + " [z " + std::to_string(var.addr) + "]"); }
        else if (var.type == Types::INT8)  { program.push_back(ld_st + "b d " + reg + " [z " + std::to_string(var.addr) + "]"); }
    }
}

void CallFunction(std::string name, const Params& params, std::vector<std::string>& program, std::unordered_map<std::string, VarData>& vars, std::unordered_map<std::string, std::vector<std::string>>& functions) {
    int j = 0;
    for (int i = 0; i < functions[name].size(); i += 2) { // Push Parameters
        if (StringToType(params.params[j]).first) {
            LdStVar(vars[params.params[j]], program, "r1", "ld");
        }
        else {
            program.push_back("li r1 " + std::to_string(StringToType(params.params[j]).second));
        }

        if      (types[functions[name][i]] == Types::INT16) { program.push_back("push r1"); }
        else if (types[functions[name][i]] == Types::INT8 ) { program.push_back("pushb r1"); }
        j++;
    }

    program.push_back("li r1 0");
    program.push_back("push r1"); // Push Return

    program.push_back("call [z " + name + "]");

    program.push_back("pop r1"); // Pop Return

    for (int i = 0; i < functions[name].size(); i += 2) { // Pop Parameters
        program.push_back("pop r2");
    }
}

enum ScopeType {
    Func, If, While
};

struct ScopeData {
    ScopeType type;
    std::string name;
};

void LoadLine(std::vector<std::vector<std::string>>& lines, int index, std::vector<std::string>& program, std::vector<std::string>& dataSegment, std::vector<ScopeData>& scopes, std::unordered_map<std::string, VarData>& vars, std::unordered_map<std::string, std::vector<std::string>>& functions, int& relitiveStackPointer, int& dataSegmentPointer) {
    std::vector<std::string>& line = lines[index];

    Types type = types[line[0]];
    std::string name = line[0];
    Params params = GetParams(line);

    if (type > 0) {
        name = line[1];

        if (line[2] == "(") { // Function Assignment
            scopes.clear();
            scopes.push_back({ ScopeType::Func, name });

            if (name == "main") {
                program.insert(program.begin() + 1, "call [z main]");
            }

            program.push_back(":" + name);
            program.push_back("push bp");
            program.push_back("mov bp sp");

            functions[name] = params.params;

            int offset = 6;
            
            vars["return"].addr = offset;
            vars["return"].isStack = true;
            if      (type == Types::INT16) { vars["return"].type = Types::INT16; offset += 2; }
            else if (type == Types::INT8 ) { vars["return"].type = Types::INT8; offset += 1; }

            for (int i = params.params.size() - 1; i >= 0; i += 2) {
                vars[params.params[i + 1]].addr = offset;
                vars[params.params[i + 1]].isStack = true;

                if      (types[params.params[i]] == Types::INT16) { vars[params.params[i + 1]].type = Types::INT16; offset += 2; }
                else if (types[params.params[i]] == Types::INT8 ) { vars[params.params[i + 1]].type = Types::INT8; offset += 1; }
            }
        }
        else { // Variable Assignment
            if (scopes.size() == 0) { // Data Variables
                vars[name] = { dataSegmentPointer, false, type };

                int assignment = 0;
                if (line.size() > 2) {
                    assignment = StringToType(line[3]).second;
                }

                switch (type)
                {
                case INT16:
                    dataSegmentPointer += 2;
                    dataSegment.push_back("%dw " + std::to_string(assignment));
                    break;
                case INT8:
                    dataSegmentPointer += 1;
                    dataSegment.push_back("%db " + std::to_string(assignment));
                    break;
                }
            }
            else { // Local Stack Variables
                vars[name] = { relitiveStackPointer, true, type };
                
                if (line.size() > 2) {
                    if (StringToType(line[3]).first) {
                        if (line.size() > 4) { // Set Function Return
                            CallFunction(line[3], params, program, vars, functions);
                        }
                        else { // Set Variable Value
                            LdStVar(vars[line[3]], program, "r1", "ld");
                        }
                    } else {
                        program.push_back("li r1 " + std::to_string(StringToType(line[3]).second));
                    }
                }

                switch (type)
                {
                case INT16:
                    relitiveStackPointer -= 2;
                    program.push_back("push r1");
                    break;
                case INT8:
                    relitiveStackPointer -= 1;
                    program.push_back("pushb r1");
                    break;
                }
            }
        }
    }
    else {
        if (line[0] == "while") {
            std::string scopeName = "while_" + std::to_string(program.size()) + "_" + params.params[0];
            scopes.push_back({ ScopeType::While, scopeName });

            program.push_back(":" + scopeName);
            
            if (params.params.size() == 3) { // Check Condition
                if (StringToType(params.params[0]).first) {
                    LdStVar(vars[params.params[0]], program, "r1", "ld");
                }
                else {
                    program.push_back("li r1 " + std::to_string(StringToType(params.params[0]).second));
                }
                if (StringToType(params.params[2]).first) {
                    LdStVar(vars[params.params[2]], program, "r2", "ld");
                }
                else {
                    program.push_back("li r2 " + std::to_string(StringToType(params.params[2]).second));
                }

                program.push_back("cmp r1 r2");

                if (params.params[1] == "==") {
                    program.push_back("jz [z " + scopeName + "_loop]");
                    program.push_back("jmp [z " + scopeName + "_end]");
                }
                else if (params.params[1] == "!=") {
                    program.push_back("jz [z " + scopeName + "_end]");
                }
            }
            else { // Check Bool
                if (StringToType(params.params[0]).first) {
                    LdStVar(vars[params.params[0]], program, "r2", "ld");
                }
                else {
                    program.push_back("li r2 " + std::to_string(StringToType(params.params[0]).second));
                }

                program.push_back("li r1 0");
                program.push_back("cmp r1 r2");
                program.push_back("jz [z " + scopeName + "_end]");
            }
            program.push_back(":" + scopeName + "_loop");
        }
        else if (line[0] == "if") {
            std::string scopeName = "if_" + std::to_string(program.size()) + "_" + params.params[0];
            scopes.push_back({ ScopeType::If, scopeName });

            if (params.params.size() == 3) { // Check Condition
                if (StringToType(params.params[0]).first) {
                    LdStVar(vars[params.params[0]], program, "r1", "ld");
                }
                else {
                    program.push_back("li r1 " + std::to_string(StringToType(params.params[0]).second));
                }
                if (StringToType(params.params[2]).first) {
                    LdStVar(vars[params.params[2]], program, "r2", "ld");
                }
                else {
                    program.push_back("li r2 " + std::to_string(StringToType(params.params[2]).second));
                }

                program.push_back("cmp r1 r2");

                if (params.params[1] == "==") {
                    program.push_back("jz [z " + scopeName + "]");
                    program.push_back("jmp [z " + scopeName + "_end]");
                }
                else if (params.params[1] == "!=") {
                    program.push_back("jz [z " + scopeName + "]");
                    program.push_back("jmp [z " + scopeName + "_end]");
                }
            }
            else { // Check Bool
                if (StringToType(params.params[0]).first) {
                    LdStVar(vars[params.params[0]], program, "r2", "ld");
                }
                else {
                    program.push_back("li r2 " + std::to_string(StringToType(params.params[0]).second));
                }

                program.push_back("li r1 0");
                program.push_back("cmp r1 r2");
                program.push_back("jz [z " + scopeName + "_end]");
            }
            program.push_back(":" + scopeName);
        }
        else if (line[0] == "return") {
            LdStVar(vars["return"], program, "r1", "st");
            program.push_back("jmp [z " + scopes[0].name + "_end]");
        }
        else if (line[0] == "asm") {
            program.push_back(params.params[0]);
        }
        else if (line[0] == "#include") {
            std::string includePath = line[2];
            std::ifstream includeFile(includePath);

            if (!includeFile.is_open()) {
                std::cout << "Unable to open include file: " << includePath << "\n";
                includeFile.close();
            }
            std::vector<std::vector<std::string>> includeLines = ParseFileLines(includeFile);
            includeFile.close();

            for (int i = 0; i < includeLines.size(); i++) {
                lines.insert(lines.begin() + index + i + 1, includeLines[i]);
            }
        }
        else if (line[0] == "}") {
            switch (scopes[scopes.size() - 1].type)
            {
            case ScopeType::Func:
                relitiveStackPointer = -1;

                program.push_back(":" + scopes[0].name + "_end");
                program.push_back("mov sp bp");
                program.push_back("pop bp");
                program.push_back("ret");
                break;
            case ScopeType::While:
                program.push_back("jmp [z " + scopes[scopes.size() - 1].name + "]");
                program.push_back(":" + scopes[scopes.size() - 1].name + "_end");
                break;
            case ScopeType::If:
                program.push_back(":" + scopes[scopes.size() - 1].name + "_end");
                break;
            }
            scopes.pop_back();
        }
        else if (line[1] == "(") { // Function Call
            CallFunction(name, params, program, vars, functions);
        }
        else { // Variable Assignment
            if (StringToType(line[2]).first) {
                if (line.size() > 3) { // Set Function Return
                    CallFunction(line[2], params, program, vars, functions);
                }
                else { // Set Variable Value
                    LdStVar(vars[line[2]], program, "r1", "ld");
                }
            } else {
                program.push_back("li r1 " + std::to_string(StringToType(line[2]).second));
            }

            if (line[1] != "=") {
                LdStVar(vars[name], program, "r2", "ld");
            }
            
            if (line[1] == "+=") {
                program.push_back("add r1 r2 r1");
            }
            else if (line[1] == "-=") {
                program.push_back("sub r1 r2 r1");
            }
            else if (line[1] == "*=") {
                program.push_back("mul r1 r2 r1");
            }
            else if (line[1] == "/=") {
                program.push_back("div r1 r2 r1");
            }
            else if (line[1] == "&=") {
                program.push_back("and r1 r2 r1");
            }
            else if (line[1] == "|=") {
                program.push_back("or r1 r2 r1");
            }

            LdStVar(vars[name], program, "r1", "st");
        }
    }
}

std::vector<std::string> CreateProgram(std::vector<std::vector<std::string>>& lines) {
    std::vector<std::string> program;
    std::vector<std::string> dataSegment;

    std::unordered_map<std::string, VarData> vars;
    std::unordered_map<std::string, std::vector<std::string>> functions;

    std::vector<ScopeData> scopes;
    int relitiveStackPointer = -1;
    int dataSegmentPointer = 0;
    std::string currentFunction;

    program.push_back("%segment code");

    for (int i = 0; i < lines.size(); i++) {
        LoadLine(lines, i, program, dataSegment, scopes, vars, functions, relitiveStackPointer, dataSegmentPointer);
    }

    program.push_back("%segment data");
    
    for (int i = 0; i < dataSegment.size(); i++) {
        program.push_back(dataSegment[i]);
    }

    for (int i = 0; i < program.size(); i++) {
        std::cout << program[i] << "\n";
    }
    std::cout << "---\n";

    return program;
}

int main(int argc, char* argv[]) {
    Paths paths = GetPaths(argc, argv);

    std::vector<std::vector<std::string>> lines = LoadFile(paths.src);

    std::vector<std::string> program = CreateProgram(lines);
    std::vector<char> binProgram = AssembleLines(program);

    std::ofstream asmFile(paths.asmb);
    std::ostream_iterator<std::string> asmFileIterator(asmFile, "\n");
    std::copy(std::begin(program), std::end(program), asmFileIterator);
    asmFile.close();

    std::ofstream outFile(paths.out, std::ios::out | std::ios::binary);
    outFile.write(&binProgram[0], binProgram.size() * sizeof(char));
    outFile.close();

    return 0;
}
