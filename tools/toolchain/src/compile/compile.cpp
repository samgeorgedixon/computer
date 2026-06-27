#include "compile.h"

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

#include "core/core.h"
#include "core/inout.h"

#define WRITE_ASM_PROGRAM(...) compileData.asmProgram.push_back({ __VA_ARGS__ })

enum Type {
    N, VOID, INT16, INT8
};
std::unordered_map<std::string, Type> types = {
    { "",           N },
    { "void",       VOID },
    { "int16_t",    INT16 },
    { "int16_tR",   INT16 },
    { "int8_t",     INT8 },
    { "int8_tR",    INT8 },
    { "char",       INT8 },
    { "charR",      INT8 },
    { "bool",       INT8 },
    { "boolR",      INT8 },
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

struct CompileData {
    std::vector<std::vector<std::string>> lines;
    std::vector<std::vector<std::string>> asmProgram;
    std::vector<ScopeData> scopes;

    std::unordered_map<std::string, VarData> vars;
    std::unordered_map<std::string, std::vector<std::string>> functions;
    std::vector<std::string> dataSegment;

    int relitiveStackPointer;
    int dataSegmentPointer;

    int mainIndex;
    int firstFuncIndex;
};

struct ParamToken {
    Type type;
    std::string name;
    std::string nameArray;
};

struct Token {
    Type type;
    std::string name;
    std::string nameArray;
    std::vector<ParamToken> params;
};

struct LexedLine {
    Token primToken;
    std::string op;
    Token secToken;
};

std::vector<ParamToken> GetParams(const std::vector<std::string>& line) {
    std::vector<ParamToken> params;
    
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

int CallFunction(CompileData& compileData, std::string name, std::vector<std::string>& params, bool cleanStack);

void SetVariable(CompileData& compileData, std::string name, std::string nameArray, std::string assignment, std::string assignmentArray, std::string assignmentOperator, std::vector<std::string>& params, std::pair<VarData, bool> nameVarOveride, std::pair<VarData, bool> assignmentVarOveride) {
    int addrOffset = 0;
    bool setVar = false;
    
    VarData& nameVar = compileData.vars[name];
    if (nameVarOveride.second) {
        nameVar = nameVarOveride.first;
    }

    VarData& assignmentVar = compileData.vars[assignment];
    if (assignmentVarOveride.second) {
        assignmentVar = assignmentVarOveride.first;
    }
    
    if (StringToImm(assignment).first || assignmentVarOveride.second) {
        if (params.size() != 0) { // Function Assignment
            int bytesToClear = CallFunction(compileData, assignment, params, false);

            if (nameArray != "") {
                if (nameVar.type == Type::INT16) {
                    WRITE_ASM_PROGRAM("pop r1");
                }
                else if (nameVar.type == Type::INT8) {
                    WRITE_ASM_PROGRAM("popb r1");
                }
            }
            else if (nameVar.count > 1) {
                for (int i = 0; i < nameVar.count; i++) {
                    if (nameVar.type == Type::INT16) {
                        WRITE_ASM_PROGRAM("pop r1");
                        LdStVar(nameVar, compileData.asmProgram, "r1", "st", i * 2);
                    }
                    else if (nameVar.type == Type::INT8) {
                        WRITE_ASM_PROGRAM("popb r1");
                        LdStVar(nameVar, compileData.asmProgram, "r1", "st", i);
                    }
                }
                setVar = true;
            }
            else {
                if (nameVar.type == Type::INT16) { WRITE_ASM_PROGRAM("pop r1"); }
                else if (nameVar.type == Type::INT8) { WRITE_ASM_PROGRAM("popb r1"); }
            }
            
            WRITE_ASM_PROGRAM("li r2 " + std::to_string(bytesToClear));
            WRITE_ASM_PROGRAM("add r2 sp r2");
            WRITE_ASM_PROGRAM("mov sp r2");
        }
        else { // Variable Assignment
            int varAddrOffset = 0;

            if (assignmentArray != "") { // Array Index
                if (StringToImm(assignmentArray).first) { // Var Index
                    if      (nameVar.type == Type::INT16) { LdStVar(compileData.vars[assignmentArray], compileData.asmProgram, "r3", "ld", 0); WRITE_ASM_PROGRAM("add r3 r3 r3") ; }
                    else if (nameVar.type == Type::INT8)  { LdStVar(compileData.vars[assignmentArray], compileData.asmProgram, "r3", "ld", 0); }

                    LdStVarOffsetReg(assignmentVar, compileData.asmProgram, "r1", "ld", "r3");
                } else { // Imm Index
                    if      (nameVar.type == Type::INT16) { varAddrOffset = StringToImm(assignmentArray).second * 2; }
                    else if (nameVar.type == Type::INT8) { varAddrOffset = StringToImm(assignmentArray).second; }
                    
                    LdStVar(assignmentVar, compileData.asmProgram, "r1", "ld", varAddrOffset);
                }
            }
            else if (nameVar.count > 1) { // Array Variable
                for (int i = 0; i < nameVar.count; i++) {
                    if (nameVar.type == Type::INT16) {
                        LdStVar(assignmentVar, compileData.asmProgram, "r1", "ld", i * 2);
                        LdStVar(nameVar, compileData.asmProgram, "r1", "st", i * 2);
                    }
                    else if (nameVar.type == Type::INT8) {
                        LdStVar(assignmentVar, compileData.asmProgram, "r1", "ld", i);
                        LdStVar(nameVar, compileData.asmProgram, "r1", "st", i);
                    }
                }
                setVar = true;
            }
            else { // Variable
                LdStVar(assignmentVar, compileData.asmProgram, "r1", "ld", varAddrOffset);
            }
        }
    } else { // Imm Assignment
        WRITE_ASM_PROGRAM("li r1 " + std::to_string(StringToImm(assignment).second));
    }

    bool arrayVarIndex = false;
    if (nameArray != "") {
        if (StringToImm(nameArray).first) {
            if      (nameVar.type == Type::INT16) { LdStVar(compileData.vars[nameArray], compileData.asmProgram, "r3", "ld", 0); WRITE_ASM_PROGRAM("add r3 r3 r3"); }
            else if (nameVar.type == Type::INT8)  { LdStVar(compileData.vars[nameArray], compileData.asmProgram, "r3", "ld", 0); }
            
            arrayVarIndex = true;
        } else {
            if      (nameVar.type == Type::INT16) { addrOffset = StringToImm(nameArray).second * 2; }
            else if (nameVar.type == Type::INT8) { addrOffset = StringToImm(nameArray).second; }
        }
    }

    if (assignmentOperator != "=") {
        if (arrayVarIndex) {
            LdStVarOffsetReg(nameVar, compileData.asmProgram, "r2", "ld", "r3");
        } else {
            LdStVar(nameVar, compileData.asmProgram, "r2", "ld", addrOffset);
        }
    }
    
    if (assignmentOperator == "+=") {
        WRITE_ASM_PROGRAM("add r1 r2 r1");
    }
    else if (assignmentOperator == "-=") {
        WRITE_ASM_PROGRAM("sub r1 r2 r1");
    }
    else if (assignmentOperator == "*=") {
        WRITE_ASM_PROGRAM("mul r1 r2 r1");
    }
    else if (assignmentOperator == "/=") {
        WRITE_ASM_PROGRAM("div r1 r2 r1");
    }
    else if (assignmentOperator == "&=") {
        WRITE_ASM_PROGRAM("and r1 r2 r1");
    }
    else if (assignmentOperator == "|=") {
        WRITE_ASM_PROGRAM("or r1 r2 r1");
    }

    if (arrayVarIndex && !setVar) {
        LdStVarOffsetReg(nameVar, compileData.asmProgram, "r1", "st", "r3");
    } else if (!setVar) {
        LdStVar(nameVar, compileData.asmProgram, "r1", "st", addrOffset);
    }
}

VarData CreateStackVar(CompileData& compileData, std::string name, Type type, std::string nameArray, std::string assignment, std::string assignmentArray, std::string assignmentOperator, std::vector<std::string>& params) {
    int count = 1;
    bool array = false;
    
    if (nameArray != "") {
        array = true;
        count = StringToImm(nameArray).second;

        if      (type == Type::INT16) { WRITE_ASM_PROGRAM("li r3 " + std::to_string(count * 2)); }
        else if (type == Type::INT8) { WRITE_ASM_PROGRAM("li r3 " + std::to_string(count)); }
        WRITE_ASM_PROGRAM("sub r2 sp r3");
        WRITE_ASM_PROGRAM("mov sp r2");
    }

    int varAddr = 0;
    
    if (!array && type == Type::INT16) { varAddr = compileData.relitiveStackPointer - 1; compileData.vars[name] = { varAddr, true, type, 1 }; }
    else if (!array && type == Type::INT8) { varAddr = compileData.relitiveStackPointer; compileData.vars[name] = { varAddr, true, type, 1 }; }
    else if (type == Type::INT16) {
        compileData.relitiveStackPointer -= count * 2; varAddr = compileData.relitiveStackPointer + 1;  compileData.vars[name] = { varAddr, true, type, count }; }
    else if (type == Type::INT8) {
        compileData.relitiveStackPointer -= count; varAddr = compileData.relitiveStackPointer + 1;  compileData.vars[name] = { varAddr, true, type, count }; }
    
    if (!array) {
        switch (type)
        {
            case INT16:
            compileData.relitiveStackPointer -= 2;
            WRITE_ASM_PROGRAM("push z");
            break;
            case INT8:
            compileData.relitiveStackPointer -= 1;
            WRITE_ASM_PROGRAM("pushb z");
            break;
        }
    }
    
    if (assignment != "") {
        SetVariable(compileData, name, "", assignment, assignmentArray, assignmentOperator, params, {{varAddr, true, type, count}, true}, {});
    }
    
    return { varAddr, true, type, count };
}

void CreateDataVar(CompileData& compileData, std::string name, Type type, std::string arrayStr, int assignment) {
    bool array = false;
    int count = 1;

    if (arrayStr != "") {
        array = true;
        count = StringToImm(arrayStr).second;
    }

    if      (!array && type == Type::INT16) { compileData.vars[name] = { compileData.dataSegmentPointer, false, type, 1 }; }
    else if (!array && type == Type::INT8) { compileData.vars[name] = { compileData.dataSegmentPointer, false, type, 1 }; }
    else if (type == Type::INT16) {
        compileData.dataSegmentPointer += count * 2; compileData.vars[name] = { compileData.dataSegmentPointer - 1, false, type, count }; }
    else if (type == Type::INT8) {
        compileData.dataSegmentPointer += count; compileData.vars[name] = { compileData.dataSegmentPointer - 1, false, type, count }; }
    

    switch (type)
    {
    case INT16:
        compileData.dataSegmentPointer += 2 * count;
        if (!array) {
            compileData.dataSegment.push_back("%dw " + std::to_string(assignment));
        }
        else {
            for (int i = 0; i < count; i++) {
                compileData.dataSegment.push_back("%dw 0");
            }
        }
        break;
    case INT8:
        compileData.dataSegmentPointer += 1 * count;
        if (!array) {
            compileData.dataSegment.push_back("%db " + std::to_string(assignment));
        }
        else {
            for (int i = 0; i < count; i++) {
                compileData.dataSegment.push_back("%db 0");
            }
        }
        break;
    }
}

int CallFunction(CompileData& compileData, std::string name, std::vector<std::string>& params, bool cleanStack) {
    int oldRelitiveStackPointer = compileData.relitiveStackPointer;
    int bytesToClear = 0;
    
    std::vector<std::string> dumbyParams;
    int j = 0;

    CompileData refSetsCompileData = compileData;
    refSetsCompileData.program.clear();
    
    for (int i = 0; i < compileData.functions[name].size(); i += 2) {
        Type paramType = types[compileData.functions[name][i]];
        std::string nameArray = GetArrayCountString(compileData.functions[name], i + 1);
        std::string assignment = "";
        std::string assignmentArray = "";
        std::string assignmentOperator = "";
        
        bool paramRef = (compileData.functions[name][i][compileData.functions[name][i].size() - 1] == 'R') ? true : false;
        
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

        VarData paramVar = CreateStackVar(compileData, "", paramType, nameArray, assignment, assignmentArray, assignmentOperator, dumbyParams);
        
        if (paramRef) {
            refSetsCompileData.relitiveStackPointer = compileData.relitiveStackPointer;
            
            SetVariable(refSetsCompileData, assignment, assignmentArray, "", "", "=", dumbyParams, {}, {paramVar, true});
        }
        
        if (j < params.size()) {
            bytesToClear = (compileData.relitiveStackPointer * -1) - (oldRelitiveStackPointer * -1);
        }

        j++;
    }
    compileData.relitiveStackPointer = oldRelitiveStackPointer;

    WRITE_ASM_PROGRAM("call [z " + name + "]");

    for (int i = 0; i < refSetsCompileData.program.size(); i++) {
        WRITE_ASM_PROGRAM(refSetsCompileData.program[i]);
    }

    if (cleanStack) {
        WRITE_ASM_PROGRAM("li r1 " + std::to_string(bytesToClear));
        WRITE_ASM_PROGRAM("add r1 sp r1");
        WRITE_ASM_PROGRAM("mov sp r1");
    }
    return bytesToClear;
}

void CreateFunction(CompileData& compileData, std::string name, Type type, std::string array, std::vector<std::string>& params) {
    compileData.scopes.clear();
    compileData.scopes.push_back({ ScopeType::Func, name, 0 });
    
    if (name == "main") {
        compileData.mainIndex = compileData.asmProgram.size() - 1;
    }
    if (compileData.functions.size() == 0) {
        compileData.firstFuncIndex = compileData.asmProgram.size() - 1;
    }

    WRITE_ASM_PROGRAM(":" + name);
    WRITE_ASM_PROGRAM("push bp");
    WRITE_ASM_PROGRAM("mov bp sp");
    
    compileData.functions[name] = params;
    
    int offset = 5;
    
    int paramsStartOffset = 1;
    if (params.size() != 0) {
        if (params[params.size() - 1] == "]") {
            paramsStartOffset += 3;
        }
    }
    
    for (int i = params.size() - 1 - paramsStartOffset; i >= 0; i -= 2) {
        int count = 1;
        bool array = false;

        if (params.size() > i + 2) {
            if (params[i + 2] == "[") {
                count = StringToImm(params[i + 3]).second;
            }
        }
        
        compileData.vars[params[i + 1]].addr = offset;
        compileData.vars[params[i + 1]].isStack = true;
        compileData.vars[params[i + 1]].count = count;
        
        if      (types[params[i]] == Type::INT16) { compileData.vars[params[i + 1]].type = Type::INT16; offset += 2 * count; }
        else if (types[params[i]] == Type::INT8 ) { compileData.vars[params[i + 1]].type = Type::INT8; offset += 1 * count; }

        if (params.size() > i + 2) {
            if (params[i + 2] == "[") {
                i -= 3;
            }
        }
    }
}

void LoadLine(CompileData& compileData, const Args& args, int lineIndex, LexedLine lexedLine) {
    std::vector<std::string>& line = compileData.lines[lineIndex];
    
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
        
        bool function = false;
        if (line.size() > assignmentIndex + 1) {
            if (line[assignmentIndex + 1] == "(") { // Function Assignment
                CreateFunction(compileData, name, type, nameArray, params);
                function = true;
            }
        }
        if (!function) { // Variable Assignment
            nameArray = GetArrayCountString(line, 1);
            if (nameArray != "") {
                assignmentIndex += 3;
            }
            assignmentIndex++;

            assignment = GetAssignment(line, assignmentIndex + 0);
            assignmentArray = GetArrayCountString(line, assignmentIndex + 1);
            assignmentOperator = GetAssignment(line, assignmentIndex - 1);

            if (compileData.scopes.size() == 0) {
                CreateDataVar(compileData, name, type, nameArray, StringToImm(assignment).second);
            }
            else {
                CreateStackVar(compileData, name, type, nameArray, assignment, assignmentArray, assignmentOperator, params);
            }
        }
    }
    else {
        if (line[0] == "while") {
            std::string scopeName = "while_" + std::to_string(compileData.asmProgram.size()) + "_" + params[0];
            compileData.scopes.push_back({ ScopeType::While, scopeName, compileData.relitiveStackPointer });

            WRITE_ASM_PROGRAM(":" + scopeName);
            
            if (params.size() == 3) { // Check Condition
                if (StringToImm(params[0]).first) {
                    LdStVar(compileData.vars[params[0]], compileData.asmProgram, "r1", "ld", 0);
                }
                else {
                    WRITE_ASM_PROGRAM("li r1 " + std::to_string(StringToImm(params[0]).second));
                }
                if (StringToImm(params[2]).first) {
                    LdStVar(compileData.vars[params[2]], compileData.asmProgram, "r2", "ld", 0);
                }
                else {
                    WRITE_ASM_PROGRAM("li r2 " + std::to_string(StringToImm(params[2]).second));
                }

                WRITE_ASM_PROGRAM("cmp r1 r2");

                if (params[1] == "==") {
                    WRITE_ASM_PROGRAM("jz [z " + scopeName + "_loop]");
                    WRITE_ASM_PROGRAM("jmp [z " + scopeName + "_end]");
                }
                else if (params[1] == "!=") {
                    WRITE_ASM_PROGRAM("jz [z " + scopeName + "_end]");
                }
            }
            else { // Check Bool
                if (StringToImm(params[0]).first) {
                    LdStVar(compileData.vars[params[0]], compileData.asmProgram, "r2", "ld", 0);
                }
                else {
                    WRITE_ASM_PROGRAM("li r2 " + std::to_string(StringToImm(params[0]).second));
                }

                WRITE_ASM_PROGRAM("li r1 0");
                WRITE_ASM_PROGRAM("cmp r1 r2");
                WRITE_ASM_PROGRAM("jz [z " + scopeName + "_end]");
            }
            WRITE_ASM_PROGRAM(":" + scopeName + "_loop");
        }
        else if (line[0] == "if") {
            std::string scopeName = "if_" + std::to_string(compileData.asmProgram.size()) + "_" + params[0];
            compileData.scopes.push_back({ ScopeType::If, scopeName, compileData.relitiveStackPointer });

            if (params.size() == 3) { // Check Condition
                if (StringToImm(params[0]).first) {
                    LdStVar(compileData.vars[params[0]], compileData.asmProgram, "r1", "ld", 0);
                }
                else {
                    WRITE_ASM_PROGRAM("li r1 " + std::to_string(StringToImm(params[0]).second));
                }
                if (StringToImm(params[2]).first) {
                    LdStVar(compileData.vars[params[2]], compileData.asmProgram, "r2", "ld", 0);
                }
                else {
                    WRITE_ASM_PROGRAM("li r2 " + std::to_string(StringToImm(params[2]).second));
                }

                WRITE_ASM_PROGRAM("cmp r1 r2");

                if (params[1] == "==") {
                    WRITE_ASM_PROGRAM("jz [z " + scopeName + "]");
                    WRITE_ASM_PROGRAM("jmp [z " + scopeName + "_end]");
                }
                else if (params[1] == "!=") {
                    WRITE_ASM_PROGRAM("jz [z " + scopeName + "_end]");
                    WRITE_ASM_PROGRAM("jmp [z " + scopeName + "]");
                }
            }
            else { // Check Bool
                if (StringToImm(params[0]).first) {
                    LdStVar(compileData.vars[params[0]], compileData.asmProgram, "r2", "ld", 0);
                }
                else {
                    WRITE_ASM_PROGRAM("li r2 " + std::to_string(StringToImm(params[0]).second));
                }

                WRITE_ASM_PROGRAM("li r1 0");
                WRITE_ASM_PROGRAM("cmp r1 r2");
                WRITE_ASM_PROGRAM("jz [z " + scopeName + "_end]");
            }
            WRITE_ASM_PROGRAM(":" + scopeName);
        }
        else if (line[0] == "return") {
            WRITE_ASM_PROGRAM("jmp [z " + compileData.scopes[0].name + "_end]");
        }
        else if (line[0] == "asm") {
            WRITE_ASM_PROGRAM(params[0]);
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
                compileData.lines.insert(compileData.lines.begin() + lineIndex + i + 1, includeLines[i]);
            }
        }
        else if (line[0] == "}") {
            compileData.relitiveStackPointer = compileData.scopes[compileData.scopes.size() - 1].relitiveStackPointer;

            switch (compileData.scopes[compileData.scopes.size() - 1].type)
            {
            case ScopeType::Func:
                WRITE_ASM_PROGRAM(":" + compileData.scopes[0].name + "_end");
                WRITE_ASM_PROGRAM("mov sp bp");
                WRITE_ASM_PROGRAM("pop bp");
                WRITE_ASM_PROGRAM("ret");

                if (compileData.scopes[0].name == "main") {
                    std::vector<std::string> main(compileData.asmProgram.begin() + compileData.mainIndex + 1, compileData.asmProgram.end());
                    
                    compileData.asmProgram.erase(compileData.asmProgram.begin() + compileData.mainIndex + 1, compileData.asmProgram.end());

                    compileData.asmProgram.insert(compileData.asmProgram.begin() + compileData.firstFuncIndex + 1, main.begin(), main.end());
                }
                break;
            case ScopeType::While:
                WRITE_ASM_PROGRAM("jmp [z " + compileData.scopes[compileData.scopes.size() - 1].name + "]");
                WRITE_ASM_PROGRAM(":" + compileData.scopes[compileData.scopes.size() - 1].name + "_end");

                WRITE_ASM_PROGRAM("li r1 " + std::to_string(compileData.scopes[compileData.scopes.size() - 1].relitiveStackPointer));
                WRITE_ASM_PROGRAM("add r1 bp r1");
                WRITE_ASM_PROGRAM("mov sp r1");
                break;
            case ScopeType::If:
                WRITE_ASM_PROGRAM(":" + compileData.scopes[compileData.scopes.size() - 1].name + "_end");

                WRITE_ASM_PROGRAM("li r1 " + std::to_string(compileData.scopes[compileData.scopes.size() - 1].relitiveStackPointer));
                WRITE_ASM_PROGRAM("add r1 bp r1");
                WRITE_ASM_PROGRAM("mov sp r1");
                break;
            }
            compileData.scopes.pop_back();
        }
        else if (line[1] == "(") {
            CallFunction(compileData, name, params, true);
        }
        else {
            SetVariable(compileData, name, nameArray, assignment, assignmentArray, assignmentOperator, params, {}, {});
        }
    }

    std::cout << type << ":" << name << ":" << nameArray << ":" << assignment << ":" << assignmentArray << ":" << assignmentOperator << "\n";
}

LexedLine LexLine(CompileData& compileData, int lineIndex) {
    std::vector<std::string>& line = compileData.lines[lineIndex];
    LexedLine lexedLine = {};
    
    lexedLine.primToken.type = types[line[0]];
    lexedLine.primToken.name = line[0];
    lexedLine.primToken.params = GetParams(line);

    std::string nameArray = GetArrayCountString(line, 0);

    int assignmentIndex = 1;
    if (nameArray != "") {
        assignmentIndex += 3;
    }

    std::string assignment = GetAssignment(line, assignmentIndex + 0);
    std::string assignmentArray = GetArrayCountString(line, assignmentIndex + 1);
    std::string assignmentOperator = GetAssignment(line, assignmentIndex - 1);

	return lexedLine;
}

void LoadLines(CompileData& compileData, const Args& args) {
    for (int i = 0; i < compileData.lines.size(); i++) {
        LexedLine lexedLine = LexLine(compileData, i);
        LoadLine(compileData, args, i, lexedLine);
    }
}

std::vector<std::vector<std::string>> CompileCLines(const std::vector<std::vector<std::string>>& lines, const Args& args) {
    CompileData compileData;

    compileData.lines = lines;
    compileData.asmProgram = {};
    compileData.scopes = {};

    compileData.vars = {};
    compileData.functions = {};
    compileData.dataSegment = {};

    compileData.relitiveStackPointer = 0;
    compileData.dataSegmentPointer = 0;

    WRITE_ASM_PROGRAM("%", "segment", "code");

    if (args.exeType == ExeType::SIZE_SEG) { WRITE_ASM_PROGRAM("%", "dw", "data"); }

    LoadLines(compileData, args);

    if (args.exeType == ExeType::SIZE_SEG) { WRITE_ASM_PROGRAM(":data"); }

    if (args.exeType == ExeType::OFST_DATA) { WRITE_ASM_PROGRAM("%", "org", std::to_string(((uint32_t)args.dataSegOffset) << 8)); }
    
    WRITE_ASM_PROGRAM("%", "segment", "data");
    
    for (int i = 0; i < compileData.dataSegment.size(); i++) {
        WRITE_ASM_PROGRAM(compileData.dataSegment[i]);
    }
    
    PrintASMProgram(compileData.asmProgram);

    return compileData.asmProgram;
}
