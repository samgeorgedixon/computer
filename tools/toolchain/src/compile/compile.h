#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "core/inout.h"

#define WRITE_LINE_ASM_PROGRAM(...) compileState.asmProgram.push_back({ __VA_ARGS__ })

enum BaseLineIdentifier {
    UNKNOWN = 0, PREPROCESSOR, WHILE, FOR, IF, ELSE_IF, ELSE, SCOPE_OPEN, SCOPE_CLOSE, RETURN, ASM
};
inline std::unordered_map<std::string, BaseLineIdentifier> baseLineIdentifiers = {
    // unknown
    { "#",          PREPROCESSOR },
    { "while",      WHILE },
    { "for",        FOR },
    { "if",         IF },
    // else if
    { "else",       ELSE },
    { "{",          SCOPE_OPEN },
    { "}",          SCOPE_CLOSE },
    { "return",     RETURN },
    { "asm",        ASM },
};

enum Type {
    NONE_TYPE = 0, VOID, INT16, INT8
};

inline std::unordered_map<std::string, Type> types = {
    // none
    { "void",       VOID },
    { "int16_t",    INT16 },
    { "int8_t",     INT8 },
    { "char",       INT8 },
    { "bool",       INT8 },
};

struct VariableData {
    int addr;
    Type type;

    bool isStack;
};

enum ScopeType {
    Func, If, While
};

struct ScopeData {
    ScopeType type;
    std::string name;
    int relitiveStackPointer;
};

struct CompileState_C {
    std::vector<std::vector<std::string>> lines;
    std::vector<std::vector<std::string>> asmProgram;
    std::vector<std::vector<std::string>> dataSegment;

    std::vector<ScopeData> scopes;

    std::unordered_map<std::string, VariableData> variables;
    std::unordered_map<std::string, std::vector<std::string>> functions;

    int relitiveStackPointer;
    int dataSegmentPointer;

    int mainIndex;
    int firstFuncIndex;
};

std::vector<std::vector<std::string>> CompileCLines(const std::vector<std::vector<std::string>>& lines, const Args& args);
