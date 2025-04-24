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
#include "core.h"

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
            else if (line[i] == '[') {
                if (isString) {
                    continue;
                }

                if (!Trim(line.substr(lastTokenEnd, i - lastTokenEnd)).empty()) {
                    lineTokens.push_back(Trim(line.substr(lastTokenEnd, i - lastTokenEnd)));
                }
                lineTokens.push_back("[");
                lastTokenEnd = i + 1;
            }
            else if (line[i] == ']') {
                if (isString) {
                    continue;
                }

                if (!Trim(line.substr(lastTokenEnd, i - lastTokenEnd)).empty()) {
                    lineTokens.push_back(Trim(line.substr(lastTokenEnd, i - lastTokenEnd)));
                }
                lineTokens.push_back("]");
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

enum Type {
    N, VOID, INT16, INT8
};
std::unordered_map<std::string, Type> types = {
    { "",           N },
    { "void",       VOID },
    { "int16_t",    INT16 },
    { "int8_t",     INT8 },
    { "char",       INT8 },
    { "bool",       INT8 },
};
std::unordered_map<Type, std::string> typesToString = {
    { N,     "" },
    { VOID,  "void" },
    { INT16, "int16_t" },
    { INT8,  "int8_t" },
};

struct VarData {
    int addr;
    bool isStack;
    Type type;
    int count;
};

enum ScopeType {
    Func, If, While
};

struct ScopeData {
    ScopeType type;
    std::string name;
    int relitiveStackPointer;
};

struct ProgramData {
    std::vector<std::vector<std::string>> lines;
    std::vector<std::string> program;
    std::vector<ScopeData> scopes;

    std::unordered_map<std::string, VarData> vars;
    std::unordered_map<std::string, std::vector<std::string>> functions;
    std::vector<std::string> dataSegment;

    int relitiveStackPointer;
    int dataSegmentPointer;

    int mainIndex;
};

std::vector<std::string> GetParams(const std::vector<std::string>& line) {
    std::vector<std::string> params;
    
    bool isParams = false;
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
    return params;
}

struct TypeString {
    bool isFunction;
    bool isVariable;
    int value;
};

std::pair<bool, int> StringToImm(std::string str) {
    if (str == "") {
        return { false, 0 };
    }
    else if (str[0] == '0' && str[1] == 'x') {
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

void LdStVar(VarData& var, std::vector<std::string>& program, std::string reg, std::string ld_st, int addrOffset) {
    if (var.isStack) {
        if      (var.type == Type::INT16) { program.push_back(ld_st + "w s " + reg + " [bp " + std::to_string(var.addr + addrOffset) + "]"); }
        else if (var.type == Type::INT8)  { program.push_back(ld_st + "b s " + reg + " [bp " + std::to_string(var.addr + addrOffset) + "]"); }
    }
    else {
        if      (var.type == Type::INT16) { program.push_back(ld_st + "w d " + reg + " [z " + std::to_string(var.addr + addrOffset) + "]"); }
        else if (var.type == Type::INT8)  { program.push_back(ld_st + "b d " + reg + " [z " + std::to_string(var.addr + addrOffset) + "]"); }
    }
}
void LdStVarOffsetReg(VarData& var, std::vector<std::string>& program, std::string reg, std::string ld_st, std::string addrOffsetReg) {
    if (var.isStack) {
        if (var.type == Type::INT16) {
            program.push_back("add " + addrOffsetReg + " " + addrOffsetReg + " " + addrOffsetReg);
            program.push_back("add " + addrOffsetReg + " " + addrOffsetReg + " bp");
            program.push_back(ld_st + "w s " + reg + " [" + addrOffsetReg + " " + std::to_string(var.addr) + "]");
        }
        else if (var.type == Type::INT8)  {
            program.push_back("add " + addrOffsetReg + " " + addrOffsetReg + " bp");
            program.push_back(ld_st + "b s " + reg + " [" + addrOffsetReg + " " + std::to_string(var.addr) + "]");
        }
    }
    else {
        program.push_back("add " + addrOffsetReg + " " + addrOffsetReg + " " + addrOffsetReg);
        if      (var.type == Type::INT16) { program.push_back(ld_st + "w d " + reg + " [" + addrOffsetReg + " " + std::to_string(var.addr) + "]"); }
        else if (var.type == Type::INT8)  { program.push_back(ld_st + "b d " + reg + " [" + addrOffsetReg + " " + std::to_string(var.addr) + "]"); }
    }
}

std::string GetArrayCountString(const std::vector<std::string>& line, int checkIndex) {
    if (line.size() <= checkIndex + 3) {
        return "";
    }

    if (line[checkIndex + 1] == "[") {
        return line[checkIndex + 2];
    }
    return "";
}

std::string GetAssignment(const std::vector<std::string>& line, int checkIndex) {
    if (line.size() <= checkIndex + 1) {
        return "";
    }
    if (line[checkIndex] != "[") {
        return line[checkIndex + 1];
    }
    return "";
}

int CallFunction(ProgramData& programData, std::string name, std::vector<std::string>& params, bool cleanStack);

void SetVariable(ProgramData& programData, std::string name, std::string nameArray, std::string assignment, std::string assignmentArray, std::string assignmentOperator, std::vector<std::string>& params, std::pair<VarData, bool> nameVarOveride) {
    int addrOffset = 0;
    bool setVar = false;
    
    VarData& nameVar = programData.vars[name];
    
    if (nameVarOveride.second) {
        nameVar = nameVarOveride.first;
    }
    
    if (StringToImm(assignment).first) {
        if (params.size() != 0) { // Function Assignment
            int bytesToClear = CallFunction(programData, assignment, params, false);

            if (nameArray != "") {
                if (nameVar.type == Type::INT16) {
                    programData.program.push_back("pop r1");
                }
                else if (nameVar.type == Type::INT8) {
                    programData.program.push_back("popb r1");
                }
            }
            else if (nameVar.count > 1) {
                for (int i = 0; i < nameVar.count; i++) {
                    if (nameVar.type == Type::INT16) {
                        programData.program.push_back("pop r1");
                        LdStVar(nameVar, programData.program, "r1", "st", i * 2);
                    }
                    else if (nameVar.type == Type::INT8) {
                        programData.program.push_back("popb r1");
                        LdStVar(nameVar, programData.program, "r1", "st", i);
                    }
                }
                setVar = true;
            }
            else {
                if (nameVar.type == Type::INT16) { programData.program.push_back("pop r1"); }
                else if (nameVar.type == Type::INT8) { programData.program.push_back("popb r1"); }
            }
            
            programData.program.push_back("li r2 " + std::to_string(bytesToClear));
            programData.program.push_back("add r2 sp r2");
            programData.program.push_back("mov sp r2");
        }
        else { // Variable Assignment
            int varAddrOffset = 0;

            if (assignmentArray != "") { // Array Index
                if (StringToImm(assignmentArray).first) { // Var Index
                    if      (nameVar.type == Type::INT16) { LdStVar(programData.vars[assignmentArray], programData.program, "r3", "ld", 0); programData.program.push_back("add r3 r3 r3") ; }
                    else if (nameVar.type == Type::INT8)  { LdStVar(programData.vars[assignmentArray], programData.program, "r3", "ld", 0); }

                    LdStVarOffsetReg(programData.vars[assignment], programData.program, "r1", "ld", "r3");
                } else { // Imm Index
                    if      (nameVar.type == Type::INT16) { varAddrOffset = StringToImm(assignmentArray).second * 2; }
                    else if (nameVar.type == Type::INT8) { varAddrOffset = StringToImm(assignmentArray).second; }
                    
                    LdStVar(programData.vars[assignment], programData.program, "r1", "ld", varAddrOffset);
                }
            }
            else if (nameVar.count > 1) { // Array Variable
                for (int i = 0; i < nameVar.count; i++) {
                    if (nameVar.type == Type::INT16) {
                        LdStVar(programData.vars[assignment], programData.program, "r1", "ld", i * 2);
                        LdStVar(nameVar, programData.program, "r1", "st", i * 2);
                    }
                    else if (nameVar.type == Type::INT8) {
                        LdStVar(programData.vars[assignment], programData.program, "r1", "ld", i);
                        LdStVar(nameVar, programData.program, "r1", "st", i);
                    }
                }
                setVar = true;
            }
            else { // Variable
                LdStVar(programData.vars[assignment], programData.program, "r1", "ld", varAddrOffset);
            }
        }
    } else { // Imm Assignment
        programData.program.push_back("li r1 " + std::to_string(StringToImm(assignment).second));
    }

    bool arrayVarIndex = false;
    if (nameArray != "") {
        if (StringToImm(nameArray).first) {
            if      (nameVar.type == Type::INT16) { LdStVar(programData.vars[nameArray], programData.program, "r3", "ld", 0); programData.program.push_back("add r3 r3 r3"); }
            else if (nameVar.type == Type::INT8)  { LdStVar(programData.vars[nameArray], programData.program, "r3", "ld", 0); }
            
            arrayVarIndex = true;
        } else {
            if      (nameVar.type == Type::INT16) { addrOffset = StringToImm(nameArray).second * 2; }
            else if (nameVar.type == Type::INT8) { addrOffset = StringToImm(nameArray).second; }
        }
    }

    if (assignmentOperator != "=") {
        if (arrayVarIndex) {
            LdStVarOffsetReg(nameVar, programData.program, "r2", "ld", "r3");
        } else {
            LdStVar(nameVar, programData.program, "r2", "ld", addrOffset);
        }
    }
    
    if (assignmentOperator == "+=") {
        programData.program.push_back("add r1 r2 r1");
    }
    else if (assignmentOperator == "-=") {
        programData.program.push_back("sub r1 r2 r1");
    }
    else if (assignmentOperator == "*=") {
        programData.program.push_back("mul r1 r2 r1");
    }
    else if (assignmentOperator == "/=") {
        programData.program.push_back("div r1 r2 r1");
    }
    else if (assignmentOperator == "&=") {
        programData.program.push_back("and r1 r2 r1");
    }
    else if (assignmentOperator == "|=") {
        programData.program.push_back("or r1 r2 r1");
    }

    if (arrayVarIndex && !setVar) {
        LdStVarOffsetReg(nameVar, programData.program, "r1", "st", "r3");
    } else if (!setVar) {
        LdStVar(nameVar, programData.program, "r1", "st", addrOffset);
    }
}

void CreateStackVar(ProgramData& programData, std::string name, Type type, std::string nameArray, std::string assignment, std::string assignmentArray, std::string assignmentOperator, std::vector<std::string>& params) {
    int count = 1;
    bool array = false;

    if (nameArray != "") {
        array = true;
        count = StringToImm(nameArray).second;

        if      (type == Type::INT16) { programData.program.push_back("li r3 " + std::to_string(count * 2)); }
        else if (type == Type::INT8) { programData.program.push_back("li r3 " + std::to_string(count)); }
        programData.program.push_back("sub r2 sp r3");
        programData.program.push_back("mov sp r2");
    }

    int varAddr = 0;
    
    if (!array && type == Type::INT16) { varAddr = programData.relitiveStackPointer - 1; programData.vars[name] = { varAddr, true, type, 1 }; }
    else if (!array && type == Type::INT8) { varAddr = programData.relitiveStackPointer; programData.vars[name] = { varAddr, true, type, 1 }; }
    else if (type == Type::INT16) {
        programData.relitiveStackPointer -= count * 2; varAddr = programData.relitiveStackPointer + 1;  programData.vars[name] = { varAddr, true, type, count }; }
    else if (type == Type::INT8) {
        programData.relitiveStackPointer -= count; varAddr = programData.relitiveStackPointer + 1;  programData.vars[name] = { varAddr, true, type, count }; }
    
    if (!array) {
        switch (type)
        {
            case INT16:
            programData.relitiveStackPointer -= 2;
            programData.program.push_back("push z");
            break;
            case INT8:
            programData.relitiveStackPointer -= 1;
            programData.program.push_back("pushb z");
            break;
        }
    }
    
    if (assignment != "") {
        SetVariable(programData, name, "", assignment, assignmentArray, assignmentOperator, params, {{varAddr, true, type, count}, true});
    }
}

void CreateDataVar(ProgramData& programData, std::string name, Type type, std::string arrayStr, int assignment) {
    bool array = false;
    int count = 1;

    if (arrayStr != "") {
        array = true;
        count = StringToImm(arrayStr).second;
    }

    if      (!array && type == Type::INT16) { programData.vars[name] = { programData.dataSegmentPointer, false, type, 1 }; }
    else if (!array && type == Type::INT8) { programData.vars[name] = { programData.dataSegmentPointer, false, type, 1 }; }
    else if (type == Type::INT16) {
        programData.dataSegmentPointer += count * 2; programData.vars[name] = { programData.dataSegmentPointer - 1, false, type, count }; }
    else if (type == Type::INT8) {
        programData.dataSegmentPointer += count; programData.vars[name] = { programData.dataSegmentPointer - 1, false, type, count }; }
    

    switch (type)
    {
    case INT16:
        programData.dataSegmentPointer += 2 * count;
        if (!array) {
            programData.dataSegment.push_back("%dw " + std::to_string(assignment));
        }
        else {
            for (int i = 0; i < count; i++) {
                programData.dataSegment.push_back("%dw 0");
            }
        }
        break;
    case INT8:
        programData.dataSegmentPointer += 1 * count;
        if (!array) {
            programData.dataSegment.push_back("%db " + std::to_string(assignment));
        }
        else {
            for (int i = 0; i < count; i++) {
                programData.dataSegment.push_back("%db 0");
            }
        }
        break;
    }
}

int CallFunction(ProgramData& programData, std::string name, std::vector<std::string>& params, bool cleanStack) {
    int oldRelitiveStackPointer = programData.relitiveStackPointer;
    int bytesToClear = 0;
    
    std::vector<std::string> dumbyParams;
    int j = 0;
    
    for (int i = 0; i < programData.functions[name].size(); i += 2) {
        Type paramType = types[programData.functions[name][i]];
        std::string nameArray = GetArrayCountString(programData.functions[name], i + 1);
        std::string assignment = "";
        std::string assignmentArray = "";
        std::string assignmentOperator = "";
        
        if (j < params.size()) {
            assignment = params[j];
            assignmentArray = GetArrayCountString(params, j);
            assignmentOperator = "=";
        
            if (assignmentArray != "") {
                j += 3;
            }
        }

        if (nameArray != "") {
            i += 3;
        }

        CreateStackVar(programData, "", paramType, nameArray, assignment, assignmentArray, assignmentOperator, dumbyParams);

        if (j < params.size()) {
            bytesToClear = (programData.relitiveStackPointer * -1) - (oldRelitiveStackPointer * -1);
        }

        j++;
    }
    programData.relitiveStackPointer = oldRelitiveStackPointer;

    programData.program.push_back("call [z " + name + "]");

    if (cleanStack) {
        programData.program.push_back("li r1 " + std::to_string(bytesToClear));
        programData.program.push_back("add r1 sp r1");
        programData.program.push_back("mov sp r1");
    }
    return bytesToClear;
}

void CreateFunction(ProgramData& programData, std::string name, Type type, std::string array, std::vector<std::string>& params) {
    programData.scopes.clear();
    programData.scopes.push_back({ ScopeType::Func, name, 0 });
    
    if (name == "main") {
        programData.mainIndex = programData.program.size() - 1;
    }

    programData.program.push_back(":" + name);
    programData.program.push_back("push bp");
    programData.program.push_back("mov bp sp");
    
    programData.functions[name] = params;

    int offset = 6;
    int rcount = 1;
    
    if (type != Type::VOID) {
        programData.functions[name].push_back(typesToString[type]);
        programData.functions[name].push_back("return");

        if (array != "") {
            rcount = StringToImm(array).second;

            programData.functions[name].push_back("[");
            programData.functions[name].push_back(array);
            programData.functions[name].push_back("]");
        }

        programData.vars["return"].addr = offset;
        programData.vars["return"].isStack = true;
        programData.vars["return"].count = rcount;

        if (type == Type::INT16) { programData.vars["return"].type = Type::INT16; offset += 2; }
        else if (type == Type::INT8) { programData.vars["return"].type = Type::INT8; offset += 1; }
    }
    
    for (int i = params.size() - 2; i >= 0; i -= 2) {
        int count = 1;
        bool array = false;

        if (params.size() > i + 2) {
            if (params[i + 2] == "[") {
                count = StringToImm(params[i + 3]).second;
            }
        }

        programData.vars[params[i + 1]].addr = offset;
        programData.vars[params[i + 1]].isStack = true;
        programData.vars[params[i + 1]].count = count;
        
        if      (types[params[i]] == Type::INT16) { programData.vars[params[i + 1]].type = Type::INT16; offset += 2 * count; }
        else if (types[params[i]] == Type::INT8 ) { programData.vars[params[i + 1]].type = Type::INT8; offset += 1 * count; }
    }
}

void LoadLine(ProgramData& programData, int lineIndex) {
    std::vector<std::string>& line = programData.lines[lineIndex];
    
    Type type = types[line[0]];
    std::string name = line[0];
    std::vector<std::string> params = GetParams(line);

    std::string nameArray = GetArrayCountString(line, 0);

    int assignmentIndex = 1;
    if (nameArray != "") {
        assignmentIndex += 3;
    }

    std::string assignment = GetAssignment(line, assignmentIndex + 0);
    std::string assignmentArray = GetArrayCountString(line, assignmentIndex + 1);
    std::string assignmentOperator = GetAssignment(line, assignmentIndex - 1);

    if (type > 0) {
        name = line[assignmentIndex];
        
        if (line[assignmentIndex + 1] == "(") { // Function Assignment
            CreateFunction(programData, name, type, nameArray, params);
        }
        else { // Variable Assignment
            nameArray = GetArrayCountString(line, 1);
            if (nameArray != "") {
                assignmentIndex += 3;
            }
            assignmentIndex++;

            assignment = GetAssignment(line, assignmentIndex + 0);
            assignmentArray = GetArrayCountString(line, assignmentIndex + 1);
            assignmentOperator = GetAssignment(line, assignmentIndex - 1);

            if (programData.scopes.size() == 0) {
                CreateDataVar(programData, name, type, nameArray, StringToImm(assignment).second);
            }
            else {
                CreateStackVar(programData, name, type, nameArray, assignment, assignmentArray, assignmentOperator, params);
            }
        }
    }
    else {
        if (line[0] == "while") {
            std::string scopeName = "while_" + std::to_string(programData.program.size()) + "_" + params[0];
            programData.scopes.push_back({ ScopeType::While, scopeName, programData.relitiveStackPointer });

            programData.program.push_back(":" + scopeName);
            
            if (params.size() == 3) { // Check Condition
                if (StringToImm(params[0]).first) {
                    LdStVar(programData.vars[params[0]], programData.program, "r1", "ld", 0);
                }
                else {
                    programData.program.push_back("li r1 " + std::to_string(StringToImm(params[0]).second));
                }
                if (StringToImm(params[2]).first) {
                    LdStVar(programData.vars[params[2]], programData.program, "r2", "ld", 0);
                }
                else {
                    programData.program.push_back("li r2 " + std::to_string(StringToImm(params[2]).second));
                }

                programData.program.push_back("cmp r1 r2");

                if (params[1] == "==") {
                    programData.program.push_back("jz [z " + scopeName + "_loop]");
                    programData.program.push_back("jmp [z " + scopeName + "_end]");
                }
                else if (params[1] == "!=") {
                    programData.program.push_back("jz [z " + scopeName + "_end]");
                }
            }
            else { // Check Bool
                if (StringToImm(params[0]).first) {
                    LdStVar(programData.vars[params[0]], programData.program, "r2", "ld", 0);
                }
                else {
                    programData.program.push_back("li r2 " + std::to_string(StringToImm(params[0]).second));
                }

                programData.program.push_back("li r1 0");
                programData.program.push_back("cmp r1 r2");
                programData.program.push_back("jz [z " + scopeName + "_end]");
            }
            programData.program.push_back(":" + scopeName + "_loop");
        }
        else if (line[0] == "if") {
            std::string scopeName = "if_" + std::to_string(programData.program.size()) + "_" + params[0];
            programData.scopes.push_back({ ScopeType::If, scopeName, programData.relitiveStackPointer });

            if (params.size() == 3) { // Check Condition
                if (StringToImm(params[0]).first) {
                    LdStVar(programData.vars[params[0]], programData.program, "r1", "ld", 0);
                }
                else {
                    programData.program.push_back("li r1 " + std::to_string(StringToImm(params[0]).second));
                }
                if (StringToImm(params[2]).first) {
                    LdStVar(programData.vars[params[2]], programData.program, "r2", "ld", 0);
                }
                else {
                    programData.program.push_back("li r2 " + std::to_string(StringToImm(params[2]).second));
                }

                programData.program.push_back("cmp r1 r2");

                if (params[1] == "==") {
                    programData.program.push_back("jz [z " + scopeName + "]");
                    programData.program.push_back("jmp [z " + scopeName + "_end]");
                }
                else if (params[1] == "!=") {
                    programData.program.push_back("jz [z " + scopeName + "_end]");
                    programData.program.push_back("jmp [z " + scopeName + "]");
                }
            }
            else { // Check Bool
                if (StringToImm(params[0]).first) {
                    LdStVar(programData.vars[params[0]], programData.program, "r2", "ld", 0);
                }
                else {
                    programData.program.push_back("li r2 " + std::to_string(StringToImm(params[0]).second));
                }

                programData.program.push_back("li r1 0");
                programData.program.push_back("cmp r1 r2");
                programData.program.push_back("jz [z " + scopeName + "_end]");
            }
            programData.program.push_back(":" + scopeName);
        }
        else if (line[0] == "return") {
            assignmentIndex--;

            assignment = GetAssignment(line, assignmentIndex + 0);
            assignmentArray = GetArrayCountString(line, assignmentIndex + 1);

            SetVariable(programData, "return", "", assignment, assignmentArray, "=", params, {});

            programData.program.push_back("jmp [z " + programData.scopes[0].name + "_end]");
        }
        else if (line[0] == "asm") {
            programData.program.push_back(params[0]);
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
                programData.lines.insert(programData.lines.begin() + lineIndex + i + 1, includeLines[i]);
            }
        }
        else if (line[0] == "}") {
            programData.relitiveStackPointer = programData.scopes[programData.scopes.size() - 1].relitiveStackPointer;

            switch (programData.scopes[programData.scopes.size() - 1].type)
            {
            case ScopeType::Func:
                programData.program.push_back(":" + programData.scopes[0].name + "_end");
                programData.program.push_back("mov sp bp");
                programData.program.push_back("pop bp");
                programData.program.push_back("ret");

                if (programData.scopes[0].name == "main") {
                    std::vector<std::string> main(programData.program.begin() + programData.mainIndex + 1, programData.program.end());
                    
                    programData.program.erase(programData.program.begin() + programData.mainIndex + 1, programData.program.end());
                    programData.program.insert(programData.program.begin() + 1, main.begin(), main.end());
                }
                break;
            case ScopeType::While:
                programData.program.push_back("jmp [z " + programData.scopes[programData.scopes.size() - 1].name + "]");
                programData.program.push_back(":" + programData.scopes[programData.scopes.size() - 1].name + "_end");

                programData.program.push_back("li sp " + std::to_string(programData.scopes[programData.scopes.size() - 1].relitiveStackPointer));
                break;
            case ScopeType::If:
                programData.program.push_back(":" + programData.scopes[programData.scopes.size() - 1].name + "_end");

                programData.program.push_back("li sp " + std::to_string(programData.scopes[programData.scopes.size() - 1].relitiveStackPointer));
                break;
            }
            programData.scopes.pop_back();
        }
        else if (line[1] == "(") {
            CallFunction(programData, name, params, true);
        }
        else {
            SetVariable(programData, name, nameArray, assignment, assignmentArray, assignmentOperator, params, {});
        }
    }

    std::cout << type << ":" << name << ":" << nameArray << ":" << assignment << ":" << assignmentArray << ":" << assignmentOperator << "\n";
}

std::vector<std::string> CreateProgram(std::vector<std::vector<std::string>>& lines) {
    ProgramData programData;

    programData.lines = lines;
    programData.program = {};
    programData.scopes = {};

    programData.vars = {};
    programData.functions = {};
    programData.dataSegment = {};

    programData.relitiveStackPointer = 0;
    programData.dataSegmentPointer = 0;

    programData.program.push_back("%segment code");

    for (int i = 0; i < programData.lines.size(); i++) {
        LoadLine(programData, i);
    }
    
    programData.program.push_back("%segment data");
    
    for (int i = 0; i < programData.dataSegment.size(); i++) {
        programData.program.push_back(programData.dataSegment[i]);
    }

    for (int i = 0; i < programData.program.size(); i++) {
        std::cout << programData.program[i] << "\n";
    }
    std::cout << "---\n";

    return programData.program;
}

int main(int argc, char* argv[]) {
    Paths paths = GetPaths(argc, argv);

#ifdef DEBUG
    paths.src = "../../programs/src/os/bootloader.c";
#endif

    std::vector<std::vector<std::string>> lines = LoadFile(paths.src);

    if (lines.size() == 0) {
        return 1;
    }

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
