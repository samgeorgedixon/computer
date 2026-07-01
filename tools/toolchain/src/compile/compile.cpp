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
#include "core/parse.h"

#include "conversion_c.h"

std::unordered_map<Type, std::string> typesToString = {
    // none
    { VOID,  "void" },
    { INT16, "int16_t" },
    { INT8,  "int8_t" }
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

//std::vector<ParamToken> GetParams(const std::vector<std::string>& line) {
//    std::vector<ParamToken> params;
//    
//    bool isParams = false;
//    int missed = 0;
//
//    for (int i = 0; i < line.size(); i++) {
//        if (line[i] == "(") {
//            isParams = true;
//        }
//        else if (line[i] == ")") {
//            for (int j = missed; j > 0; j--) {
//                params.push_back(line[i - j]);
//            }
//            missed = 0;
//        }
//        else if (line[i] == ",") {
//            for (int j = missed; j > 0; j--) {
//                params.push_back(line[i - j]);
//            }
//            missed = 0;
//        }
//        else if (line[i] == "\"") {
//            i++;
//            params.push_back(line[i]);
//            i++;
//
//            missed = 0;
//        }
//        else {
//            if (isParams) {
//                missed++;
//            }
//        }
//    }
//
//    if (!isParams) {
//        params = {};
//    }
//    return params;
//}
//
//struct TypeString {
//    bool isFunction;
//    bool isVariable;
//    int value;
//};
//
//std::pair<bool, int> StringToImm(std::string str) {
//    if (str == "") {
//        return { false, 0 };
//    }
//    else if (str[0] == '0' && str[1] == 'x') {
//        return { false, std::stoi(str.substr(2), nullptr, 16) };
//    }
//    else if (str[0] == '0' && str[1] == 'b') {
//        return { false, std::stoi(str.substr(2), nullptr, 2) };
//    }
//    else if (str[0] == '\'') {
//        return { false, str[1] };
//    }
//    else if (str == "true") {
//        return { false, true };
//    }
//    else if (str == "false") {
//        return { false, false };
//    }
//    else if (IsStrAlpha(str)) {
//        return { true, 0 };
//    }
//    else {
//        return { false, std::stoi(str) };
//    }
//    return { false, 0 };
//}
//
//void LdStVar(VarData& var, std::vector<std::string>& program, std::string reg, std::string ld_st, int addrOffset) {
//    if (var.isStack) {
//        if      (var.type == Type::INT16) { program.push_back(ld_st + "w s " + reg + " [bp " + std::to_string(var.addr + addrOffset) + "]"); }
//        else if (var.type == Type::INT8)  { program.push_back(ld_st + "b s " + reg + " [bp " + std::to_string(var.addr + addrOffset) + "]"); }
//    }
//    else {
//        if      (var.type == Type::INT16) { program.push_back(ld_st + "w d " + reg + " [z " + std::to_string(var.addr + addrOffset) + "]"); }
//        else if (var.type == Type::INT8)  { program.push_back(ld_st + "b d " + reg + " [z " + std::to_string(var.addr + addrOffset) + "]"); }
//    }
//}
//void LdStVarOffsetReg(VarData& var, std::vector<std::string>& program, std::string reg, std::string ld_st, std::string addrOffsetReg) {
//    if (var.isStack) {
//        if (var.type == Type::INT16) {
//            program.push_back("add " + addrOffsetReg + " " + addrOffsetReg + " " + addrOffsetReg);
//            program.push_back("add " + addrOffsetReg + " " + addrOffsetReg + " bp");
//            program.push_back(ld_st + "w s " + reg + " [" + addrOffsetReg + " " + std::to_string(var.addr) + "]");
//        }
//        else if (var.type == Type::INT8)  {
//            program.push_back("add " + addrOffsetReg + " " + addrOffsetReg + " bp");
//            program.push_back(ld_st + "b s " + reg + " [" + addrOffsetReg + " " + std::to_string(var.addr) + "]");
//        }
//    }
//    else {
//        program.push_back("add " + addrOffsetReg + " " + addrOffsetReg + " " + addrOffsetReg);
//        if      (var.type == Type::INT16) { program.push_back(ld_st + "w d " + reg + " [" + addrOffsetReg + " " + std::to_string(var.addr) + "]"); }
//        else if (var.type == Type::INT8)  { program.push_back(ld_st + "b d " + reg + " [" + addrOffsetReg + " " + std::to_string(var.addr) + "]"); }
//    }
//}
//
//std::string GetArrayCountString(const std::vector<std::string>& line, int checkIndex) {
//    if (line.size() <= checkIndex + 3) {
//        return "";
//    }
//
//    if (line[checkIndex + 1] == "[") {
//        return line[checkIndex + 2];
//    }
//    return "";
//}
//
//std::string GetAssignment(const std::vector<std::string>& line, int checkIndex) {
//    if (line.size() <= checkIndex + 1) {
//        return "";
//    }
//    if (line[checkIndex] != "[") {
//        return line[checkIndex + 1];
//    }
//    return "";
//}
//
//int CallFunction(CompileState_C& compileState, std::string name, std::vector<std::string>& params, bool cleanStack);
//
//void SetVariable(CompileState_C& compileState, std::string name, std::string nameArray, std::string assignment, std::string assignmentArray, std::string assignmentExpressionOperator, std::vector<std::string>& params, std::pair<VarData, bool> nameVarOveride, std::pair<VarData, bool> assignmentVarOveride) {
//    int addrOffset = 0;
//    bool setVar = false;
//    
//    VarData& nameVar = compileState.vars[name];
//    if (nameVarOveride.second) {
//        nameVar = nameVarOveride.first;
//    }
//
//    VarData& assignmentVar = compileState.vars[assignment];
//    if (assignmentVarOveride.second) {
//        assignmentVar = assignmentVarOveride.first;
//    }
//    
//    if (StringToImm(assignment).first || assignmentVarOveride.second) {
//        if (params.size() != 0) { // Function Assignment
//            int bytesToClear = CallFunction(compileState, assignment, params, false);
//
//            if (nameArray != "") {
//                if (nameVar.type == Type::INT16) {
//                    WRITE_LINE_ASM_PROGRAM("pop r1");
//                }
//                else if (nameVar.type == Type::INT8) {
//                    WRITE_LINE_ASM_PROGRAM("popb r1");
//                }
//            }
//            else if (nameVar.count > 1) {
//                for (int i = 0; i < nameVar.count; i++) {
//                    if (nameVar.type == Type::INT16) {
//                        WRITE_LINE_ASM_PROGRAM("pop r1");
//                        LdStVar(nameVar, compileState.asmProgram, "r1", "st", i * 2);
//                    }
//                    else if (nameVar.type == Type::INT8) {
//                        WRITE_LINE_ASM_PROGRAM("popb r1");
//                        LdStVar(nameVar, compileState.asmProgram, "r1", "st", i);
//                    }
//                }
//                setVar = true;
//            }
//            else {
//                if (nameVar.type == Type::INT16) { WRITE_LINE_ASM_PROGRAM("pop r1"); }
//                else if (nameVar.type == Type::INT8) { WRITE_LINE_ASM_PROGRAM("popb r1"); }
//            }
//            
//            WRITE_LINE_ASM_PROGRAM("li r2 " + std::to_string(bytesToClear));
//            WRITE_LINE_ASM_PROGRAM("add r2 sp r2");
//            WRITE_LINE_ASM_PROGRAM("mov sp r2");
//        }
//        else { // Variable Assignment
//            int varAddrOffset = 0;
//
//            if (assignmentArray != "") { // Array Index
//                if (StringToImm(assignmentArray).first) { // Var Index
//                    if      (nameVar.type == Type::INT16) { LdStVar(compileState.vars[assignmentArray], compileState.asmProgram, "r3", "ld", 0); WRITE_LINE_ASM_PROGRAM("add r3 r3 r3") ; }
//                    else if (nameVar.type == Type::INT8)  { LdStVar(compileState.vars[assignmentArray], compileState.asmProgram, "r3", "ld", 0); }
//
//                    LdStVarOffsetReg(assignmentVar, compileState.asmProgram, "r1", "ld", "r3");
//                } else { // Imm Index
//                    if      (nameVar.type == Type::INT16) { varAddrOffset = StringToImm(assignmentArray).second * 2; }
//                    else if (nameVar.type == Type::INT8) { varAddrOffset = StringToImm(assignmentArray).second; }
//                    
//                    LdStVar(assignmentVar, compileState.asmProgram, "r1", "ld", varAddrOffset);
//                }
//            }
//            else if (nameVar.count > 1) { // Array Variable
//                for (int i = 0; i < nameVar.count; i++) {
//                    if (nameVar.type == Type::INT16) {
//                        LdStVar(assignmentVar, compileState.asmProgram, "r1", "ld", i * 2);
//                        LdStVar(nameVar, compileState.asmProgram, "r1", "st", i * 2);
//                    }
//                    else if (nameVar.type == Type::INT8) {
//                        LdStVar(assignmentVar, compileState.asmProgram, "r1", "ld", i);
//                        LdStVar(nameVar, compileState.asmProgram, "r1", "st", i);
//                    }
//                }
//                setVar = true;
//            }
//            else { // Variable
//                LdStVar(assignmentVar, compileState.asmProgram, "r1", "ld", varAddrOffset);
//            }
//        }
//    } else { // Imm Assignment
//        WRITE_LINE_ASM_PROGRAM("li r1 " + std::to_string(StringToImm(assignment).second));
//    }
//
//    bool arrayVarIndex = false;
//    if (nameArray != "") {
//        if (StringToImm(nameArray).first) {
//            if      (nameVar.type == Type::INT16) { LdStVar(compileState.vars[nameArray], compileState.asmProgram, "r3", "ld", 0); WRITE_LINE_ASM_PROGRAM("add r3 r3 r3"); }
//            else if (nameVar.type == Type::INT8)  { LdStVar(compileState.vars[nameArray], compileState.asmProgram, "r3", "ld", 0); }
//            
//            arrayVarIndex = true;
//        } else {
//            if      (nameVar.type == Type::INT16) { addrOffset = StringToImm(nameArray).second * 2; }
//            else if (nameVar.type == Type::INT8) { addrOffset = StringToImm(nameArray).second; }
//        }
//    }
//
//    if (assignmentExpressionOperator != "=") {
//        if (arrayVarIndex) {
//            LdStVarOffsetReg(nameVar, compileState.asmProgram, "r2", "ld", "r3");
//        } else {
//            LdStVar(nameVar, compileState.asmProgram, "r2", "ld", addrOffset);
//        }
//    }
//    
//    if (assignmentExpressionOperator == "+=") {
//        WRITE_LINE_ASM_PROGRAM("add r1 r2 r1");
//    }
//    else if (assignmentExpressionOperator == "-=") {
//        WRITE_LINE_ASM_PROGRAM("sub r1 r2 r1");
//    }
//    else if (assignmentExpressionOperator == "*=") {
//        WRITE_LINE_ASM_PROGRAM("mul r1 r2 r1");
//    }
//    else if (assignmentExpressionOperator == "/=") {
//        WRITE_LINE_ASM_PROGRAM("div r1 r2 r1");
//    }
//    else if (assignmentExpressionOperator == "&=") {
//        WRITE_LINE_ASM_PROGRAM("and r1 r2 r1");
//    }
//    else if (assignmentExpressionOperator == "|=") {
//        WRITE_LINE_ASM_PROGRAM("or r1 r2 r1");
//    }
//
//    if (arrayVarIndex && !setVar) {
//        LdStVarOffsetReg(nameVar, compileState.asmProgram, "r1", "st", "r3");
//    } else if (!setVar) {
//        LdStVar(nameVar, compileState.asmProgram, "r1", "st", addrOffset);
//    }
//}
//
//VarData CreateStackVar(CompileState_C& compileState, std::string name, Type type, std::string nameArray, std::string assignment, std::string assignmentArray, std::string assignmentExpressionOperator, std::vector<std::string>& params) {
//    int count = 1;
//    bool array = false;
//    
//    if (nameArray != "") {
//        array = true;
//        count = StringToImm(nameArray).second;
//
//        if      (type == Type::INT16) { WRITE_LINE_ASM_PROGRAM("li r3 " + std::to_string(count * 2)); }
//        else if (type == Type::INT8) { WRITE_LINE_ASM_PROGRAM("li r3 " + std::to_string(count)); }
//        WRITE_LINE_ASM_PROGRAM("sub r2 sp r3");
//        WRITE_LINE_ASM_PROGRAM("mov sp r2");
//    }
//
//    int varAddr = 0;
//    
//    if (!array && type == Type::INT16) { varAddr = compileState.relitiveStackPointer - 1; compileState.vars[name] = { varAddr, true, type, 1 }; }
//    else if (!array && type == Type::INT8) { varAddr = compileState.relitiveStackPointer; compileState.vars[name] = { varAddr, true, type, 1 }; }
//    else if (type == Type::INT16) {
//        compileState.relitiveStackPointer -= count * 2; varAddr = compileState.relitiveStackPointer + 1;  compileState.vars[name] = { varAddr, true, type, count }; }
//    else if (type == Type::INT8) {
//        compileState.relitiveStackPointer -= count; varAddr = compileState.relitiveStackPointer + 1;  compileState.vars[name] = { varAddr, true, type, count }; }
//    
//    if (!array) {
//        switch (type)
//        {
//            case INT16:
//            compileState.relitiveStackPointer -= 2;
//            WRITE_LINE_ASM_PROGRAM("push z");
//            break;
//            case INT8:
//            compileState.relitiveStackPointer -= 1;
//            WRITE_LINE_ASM_PROGRAM("pushb z");
//            break;
//        }
//    }
//    
//    if (assignment != "") {
//        SetVariable(compileState, name, "", assignment, assignmentArray, assignmentExpressionOperator, params, {{varAddr, true, type, count}, true}, {});
//    }
//    
//    return { varAddr, true, type, count };
//}
//
//void CreateDataVar(CompileState_C& compileState, std::string name, Type type, std::string arrayStr, int assignment) {
//    bool array = false;
//    int count = 1;
//
//    if (arrayStr != "") {
//        array = true;
//        count = StringToImm(arrayStr).second;
//    }
//
//    if      (!array && type == Type::INT16) { compileState.vars[name] = { compileState.dataSegmentPointer, false, type, 1 }; }
//    else if (!array && type == Type::INT8) { compileState.vars[name] = { compileState.dataSegmentPointer, false, type, 1 }; }
//    else if (type == Type::INT16) {
//        compileState.dataSegmentPointer += count * 2; compileState.vars[name] = { compileState.dataSegmentPointer - 1, false, type, count }; }
//    else if (type == Type::INT8) {
//        compileState.dataSegmentPointer += count; compileState.vars[name] = { compileState.dataSegmentPointer - 1, false, type, count }; }
//    
//
//    switch (type)
//    {
//    case INT16:
//        compileState.dataSegmentPointer += 2 * count;
//        if (!array) {
//            compileState.dataSegment.push_back("%dw " + std::to_string(assignment));
//        }
//        else {
//            for (int i = 0; i < count; i++) {
//                compileState.dataSegment.push_back("%dw 0");
//            }
//        }
//        break;
//    case INT8:
//        compileState.dataSegmentPointer += 1 * count;
//        if (!array) {
//            compileState.dataSegment.push_back("%db " + std::to_string(assignment));
//        }
//        else {
//            for (int i = 0; i < count; i++) {
//                compileState.dataSegment.push_back("%db 0");
//            }
//        }
//        break;
//    }
//}
//
//int CallFunction(CompileState_C& compileState, std::string name, std::vector<std::string>& params, bool cleanStack) {
//    int oldRelitiveStackPointer = compileState.relitiveStackPointer;
//    int bytesToClear = 0;
//    
//    std::vector<std::string> dumbyParams;
//    int j = 0;
//
//    CompileState_C refSetsCompileState_C = compileState;
//    refSetsCompileState_C.asmProgram.clear();
//    
//    for (int i = 0; i < compileState.functions[name].size(); i += 2) {
//        Type paramType = types[compileState.functions[name][i]];
//        std::string nameArray = GetArrayCountString(compileState.functions[name], i + 1);
//        std::string assignment = "";
//        std::string assignmentArray = "";
//        std::string assignmentExpressionOperator = "";
//        
//        bool paramRef = (compileState.functions[name][i][compileState.functions[name][i].size() - 1] == 'R') ? true : false;
//        
//        if (j < params.size()) {
//            assignment = params[j];
//            assignmentArray = GetArrayCountString(params, j);
//            assignmentExpressionOperator = "=";
//        
//            if (assignmentArray != "") {
//                j += 3;
//            }
//        }
//
//        if (nameArray != "") {
//            i += 3;
//        }
//
//        VarData paramVar = CreateStackVar(compileState, "", paramType, nameArray, assignment, assignmentArray, assignmentExpressionOperator, dumbyParams);
//        
//        if (paramRef) {
//            refSetsCompileState_C.relitiveStackPointer = compileState.relitiveStackPointer;
//            
//            SetVariable(refSetsCompileState_C, assignment, assignmentArray, "", "", "=", dumbyParams, {}, {paramVar, true});
//        }
//        
//        if (j < params.size()) {
//            bytesToClear = (compileState.relitiveStackPointer * -1) - (oldRelitiveStackPointer * -1);
//        }
//
//        j++;
//    }
//    compileState.relitiveStackPointer = oldRelitiveStackPointer;
//
//    WRITE_LINE_ASM_PROGRAM("call [z " + name + "]");
//
//    for (int i = 0; i < refSetsCompileState_C.asmProgram.size(); i++) {
//        WRITE_LINE_ASM_PROGRAM(refSetsCompileState_C.asmProgram[i]);
//    }
//
//    if (cleanStack) {
//        WRITE_LINE_ASM_PROGRAM("li r1 " + std::to_string(bytesToClear));
//        WRITE_LINE_ASM_PROGRAM("add r1 sp r1");
//        WRITE_LINE_ASM_PROGRAM("mov sp r1");
//    }
//    return bytesToClear;
//}
//
//void CreateFunction(CompileState_C& compileState, std::string name, Type type, std::string array, std::vector<std::string>& params) {
//    compileState.scopes.clear();
//    compileState.scopes.push_back({ ScopeType::Func, name, 0 });
//    
//    if (name == "main") {
//        compileState.mainIndex = compileState.asmProgram.size() - 1;
//    }
//    if (compileState.functions.size() == 0) {
//        compileState.firstFuncIndex = compileState.asmProgram.size() - 1;
//    }
//
//    WRITE_LINE_ASM_PROGRAM(":" + name);
//    WRITE_LINE_ASM_PROGRAM("push bp");
//    WRITE_LINE_ASM_PROGRAM("mov bp sp");
//    
//    compileState.functions[name] = params;
//    
//    int offset = 5;
//    
//    int paramsStartOffset = 1;
//    if (params.size() != 0) {
//        if (params[params.size() - 1] == "]") {
//            paramsStartOffset += 3;
//        }
//    }
//    
//    for (int i = params.size() - 1 - paramsStartOffset; i >= 0; i -= 2) {
//        int count = 1;
//        bool array = false;
//
//        if (params.size() > i + 2) {
//            if (params[i + 2] == "[") {
//                count = StringToImm(params[i + 3]).second;
//            }
//        }
//        
//        compileState.vars[params[i + 1]].addr = offset;
//        compileState.vars[params[i + 1]].isStack = true;
//        compileState.vars[params[i + 1]].count = count;
//        
//        if      (types[params[i]] == Type::INT16) { compileState.vars[params[i + 1]].type = Type::INT16; offset += 2 * count; }
//        else if (types[params[i]] == Type::INT8 ) { compileState.vars[params[i + 1]].type = Type::INT8; offset += 1 * count; }
//
//        if (params.size() > i + 2) {
//            if (params[i + 2] == "[") {
//                i -= 3;
//            }
//        }
//    }
//}
//
//void LoadLine(CompileState_C& compileState, int lineIndex, LexedLine lexedLine) {
//    std::vector<std::string>& line = compileState.lines[lineIndex];
//    
//    Type type = types[line[0]];
//    std::string name = line[0];
//    std::vector<std::string> params = GetParams(line);
//
//    std::string nameArray = GetArrayCountString(line, 0);
//
//    int assignmentIndex = 1;
//    if (nameArray != "") {
//        assignmentIndex += 3;
//    }
//
//    std::string assignment = GetAssignment(line, assignmentIndex + 0);
//    std::string assignmentArray = GetArrayCountString(line, assignmentIndex + 1);
//    std::string assignmentExpressionOperator = GetAssignment(line, assignmentIndex - 1);
//
//    if (type > 0) {
//        name = line[assignmentIndex];
//        
//        bool function = false;
//        if (line.size() > assignmentIndex + 1) {
//            if (line[assignmentIndex + 1] == "(") { // Function Assignment
//                CreateFunction(compileState, name, type, nameArray, params);
//                function = true;
//            }
//        }
//        if (!function) { // Variable Assignment
//            nameArray = GetArrayCountString(line, 1);
//            if (nameArray != "") {
//                assignmentIndex += 3;
//            }
//            assignmentIndex++;
//
//            assignment = GetAssignment(line, assignmentIndex + 0);
//            assignmentArray = GetArrayCountString(line, assignmentIndex + 1);
//            assignmentExpressionOperator = GetAssignment(line, assignmentIndex - 1);
//
//            if (compileState.scopes.size() == 0) {
//                CreateDataVar(compileState, name, type, nameArray, StringToImm(assignment).second);
//            }
//            else {
//                CreateStackVar(compileState, name, type, nameArray, assignment, assignmentArray, assignmentExpressionOperator, params);
//            }
//        }
//    }
//    else {
//        if (line[0] == "while") {
//            std::string scopeName = "while_" + std::to_string(compileState.asmProgram.size()) + "_" + params[0];
//            compileState.scopes.push_back({ ScopeType::While, scopeName, compileState.relitiveStackPointer });
//
//            WRITE_LINE_ASM_PROGRAM(":" + scopeName);
//            
//            if (params.size() == 3) { // Check Condition
//                if (StringToImm(params[0]).first) {
//                    LdStVar(compileState.vars[params[0]], compileState.asmProgram, "r1", "ld", 0);
//                }
//                else {
//                    WRITE_LINE_ASM_PROGRAM("li r1 " + std::to_string(StringToImm(params[0]).second));
//                }
//                if (StringToImm(params[2]).first) {
//                    LdStVar(compileState.vars[params[2]], compileState.asmProgram, "r2", "ld", 0);
//                }
//                else {
//                    WRITE_LINE_ASM_PROGRAM("li r2 " + std::to_string(StringToImm(params[2]).second));
//                }
//
//                WRITE_LINE_ASM_PROGRAM("cmp r1 r2");
//
//                if (params[1] == "==") {
//                    WRITE_LINE_ASM_PROGRAM("jz [z " + scopeName + "_loop]");
//                    WRITE_LINE_ASM_PROGRAM("jmp [z " + scopeName + "_end]");
//                }
//                else if (params[1] == "!=") {
//                    WRITE_LINE_ASM_PROGRAM("jz [z " + scopeName + "_end]");
//                }
//            }
//            else { // Check Bool
//                if (StringToImm(params[0]).first) {
//                    LdStVar(compileState.vars[params[0]], compileState.asmProgram, "r2", "ld", 0);
//                }
//                else {
//                    WRITE_LINE_ASM_PROGRAM("li r2 " + std::to_string(StringToImm(params[0]).second));
//                }
//
//                WRITE_LINE_ASM_PROGRAM("li r1 0");
//                WRITE_LINE_ASM_PROGRAM("cmp r1 r2");
//                WRITE_LINE_ASM_PROGRAM("jz [z " + scopeName + "_end]");
//            }
//            WRITE_LINE_ASM_PROGRAM(":" + scopeName + "_loop");
//        }
//        else if (line[0] == "if") {
//            std::string scopeName = "if_" + std::to_string(compileState.asmProgram.size()) + "_" + params[0];
//            compileState.scopes.push_back({ ScopeType::If, scopeName, compileState.relitiveStackPointer });
//
//            if (params.size() == 3) { // Check Condition
//                if (StringToImm(params[0]).first) {
//                    LdStVar(compileState.vars[params[0]], compileState.asmProgram, "r1", "ld", 0);
//                }
//                else {
//                    WRITE_LINE_ASM_PROGRAM("li r1 " + std::to_string(StringToImm(params[0]).second));
//                }
//                if (StringToImm(params[2]).first) {
//                    LdStVar(compileState.vars[params[2]], compileState.asmProgram, "r2", "ld", 0);
//                }
//                else {
//                    WRITE_LINE_ASM_PROGRAM("li r2 " + std::to_string(StringToImm(params[2]).second));
//                }
//
//                WRITE_LINE_ASM_PROGRAM("cmp r1 r2");
//
//                if (params[1] == "==") {
//                    WRITE_LINE_ASM_PROGRAM("jz [z " + scopeName + "]");
//                    WRITE_LINE_ASM_PROGRAM("jmp [z " + scopeName + "_end]");
//                }
//                else if (params[1] == "!=") {
//                    WRITE_LINE_ASM_PROGRAM("jz [z " + scopeName + "_end]");
//                    WRITE_LINE_ASM_PROGRAM("jmp [z " + scopeName + "]");
//                }
//            }
//            else { // Check Bool
//                if (StringToImm(params[0]).first) {
//                    LdStVar(compileState.vars[params[0]], compileState.asmProgram, "r2", "ld", 0);
//                }
//                else {
//                    WRITE_LINE_ASM_PROGRAM("li r2 " + std::to_string(StringToImm(params[0]).second));
//                }
//
//                WRITE_LINE_ASM_PROGRAM("li r1 0");
//                WRITE_LINE_ASM_PROGRAM("cmp r1 r2");
//                WRITE_LINE_ASM_PROGRAM("jz [z " + scopeName + "_end]");
//            }
//            WRITE_LINE_ASM_PROGRAM(":" + scopeName);
//        }
//        else if (line[0] == "return") {
//            WRITE_LINE_ASM_PROGRAM("jmp [z " + compileState.scopes[0].name + "_end]");
//        }
//        else if (line[0] == "asm") {
//            WRITE_LINE_ASM_PROGRAM(params[0]);
//        }
//        else if (line[0] == "#include") {
//            std::string includePath = line[2];
//
//            std::vector<std::vector<std::string>> includeLines = ParseCFile(includePath);
//
//            for (int i = 0; i < includeLines.size(); i++) {
//                compileState.lines.insert(compileState.lines.begin() + lineIndex + i + 1, includeLines[i]);
//            }
//        }
//        else if (line[0] == "}") {
//            compileState.relitiveStackPointer = compileState.scopes[compileState.scopes.size() - 1].relitiveStackPointer;
//
//            switch (compileState.scopes[compileState.scopes.size() - 1].type)
//            {
//            case ScopeType::Func:
//                WRITE_LINE_ASM_PROGRAM(":" + compileState.scopes[0].name + "_end");
//                WRITE_LINE_ASM_PROGRAM("mov sp bp");
//                WRITE_LINE_ASM_PROGRAM("pop bp");
//                WRITE_LINE_ASM_PROGRAM("ret");
//
//                if (compileState.scopes[0].name == "main") {
//                    std::vector<std::string> main(compileState.asmProgram.begin() + compileState.mainIndex + 1, compileState.asmProgram.end());
//                    
//                    compileState.asmProgram.erase(compileState.asmProgram.begin() + compileState.mainIndex + 1, compileState.asmProgram.end());
//
//                    compileState.asmProgram.insert(compileState.asmProgram.begin() + compileState.firstFuncIndex + 1, main.begin(), main.end());
//                }
//                break;
//            case ScopeType::While:
//                WRITE_LINE_ASM_PROGRAM("jmp [z " + compileState.scopes[compileState.scopes.size() - 1].name + "]");
//                WRITE_LINE_ASM_PROGRAM(":" + compileState.scopes[compileState.scopes.size() - 1].name + "_end");
//
//                WRITE_LINE_ASM_PROGRAM("li r1 " + std::to_string(compileState.scopes[compileState.scopes.size() - 1].relitiveStackPointer));
//                WRITE_LINE_ASM_PROGRAM("add r1 bp r1");
//                WRITE_LINE_ASM_PROGRAM("mov sp r1");
//                break;
//            case ScopeType::If:
//                WRITE_LINE_ASM_PROGRAM(":" + compileState.scopes[compileState.scopes.size() - 1].name + "_end");
//
//                WRITE_LINE_ASM_PROGRAM("li r1 " + std::to_string(compileState.scopes[compileState.scopes.size() - 1].relitiveStackPointer));
//                WRITE_LINE_ASM_PROGRAM("add r1 bp r1");
//                WRITE_LINE_ASM_PROGRAM("mov sp r1");
//                break;
//            }
//            compileState.scopes.pop_back();
//        }
//        else if (line[1] == "(") {
//            CallFunction(compileState, name, params, true);
//        }
//        else {
//            SetVariable(compileState, name, nameArray, assignment, assignmentArray, assignmentExpressionOperator, params, {}, {});
//        }
//    }
//
//    std::cout << type << ":" << name << ":" << nameArray << ":" << assignment << ":" << assignmentArray << ":" << assignmentExpressionOperator << "\n";
//}
//
//LexedLine LexLine(CompileState_C& compileState, int lineIndex) {
//    std::vector<std::string>& line = compileState.lines[lineIndex];
//    LexedLine lexedLine = {};
//    
//    lexedLine.primToken.type = types[line[0]];
//    lexedLine.primToken.name = line[0];
//    lexedLine.primToken.params = GetParams(line);
//
//    std::string nameArray = GetArrayCountString(line, 0);
//
//    int assignmentIndex = 1;
//    if (nameArray != "") {
//        assignmentIndex += 3;
//    }
//
//    std::string assignment = GetAssignment(line, assignmentIndex + 0);
//    std::string assignmentArray = GetArrayCountString(line, assignmentIndex + 1);
//    std::string assignmentExpressionOperator = GetAssignment(line, assignmentIndex - 1);
//
//	return lexedLine;
//}

void WriteDataSegment(CompileState_C& compileState) {
    for (int i = 0; i < compileState.dataSegment.size(); i++) {
        WRITE_LINE_ASM_PROGRAM(compileState.dataSegment[i]);
    }
}

void WriteASMProgram(CompileState_C& compileState, const Args& args) {
    WRITE_LINE_ASM_PROGRAM("%", "segment", "code");

    if (args.exeType == ExeType::SIZE_SEG) { WRITE_LINE_ASM_PROGRAM("%", "dw", "data"); }

    ConvertLines(compileState);

    if (args.exeType == ExeType::SIZE_SEG) { WRITE_LINE_ASM_PROGRAM(":data"); }
    if (args.exeType == ExeType::OFST_DATA) { WRITE_LINE_ASM_PROGRAM("%", "org", std::to_string(((uint32_t)args.dataSegOffset) << 8)); }

    WRITE_LINE_ASM_PROGRAM("%", "segment", "data");

	WriteDataSegment(compileState);
}

std::vector<std::vector<std::string>> CompileCLines(const std::vector<std::vector<std::string>>& lines, const Args& args) {
    CompileState_C compileState;

    compileState.lines = lines;
    compileState.asmProgram = {};
    compileState.dataSegment = {};

    compileState.scopes = {};

    compileState.variables = {};
    compileState.functions = {};

    compileState.relitiveStackPointer = 0;
    compileState.dataSegmentPointer = 0;

	WriteASMProgram(compileState, args);

    return compileState.asmProgram;
}
