#pragma once
#include <string>
#include <unordered_map>

#include "compile.h"

inline std::unordered_map<std::string, bool> builtInRegisters {
    // none
    {"z", true},

    {"r1", true},
    {"r2", true},
    {"r3", true},
    {"r4", true},

    {"sp", true},
    {"bp", true},

    {"cs", true},
    {"ds", true},
    {"ss", true},
    {"es", true},

    {"ip", true},
    {"ir", true},
    {"ar", true},
};

void DeclareStackVariable(CompileState_C& compileState, std::string name, Type type, int arrayCount);

void DeclareDataVariable(CompileState_C& compileState, std::string name, Type type, int arrayCount);

void LoadFromORStoreToVariable(CompileState_C& compileState, bool isLoadFrom, std::string variableName, int arrayIndex, std::string srcORdestRegisterName);
