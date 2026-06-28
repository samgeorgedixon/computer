#include "conversion_asm.h"

#include <unordered_map>
#include <iostream>

#include "core/core.h"
#include "assemble.h"

uint16_t ConvertInstrParam(AssembleState& assembleState, std::string instrParamStr) {
    uint16_t instrParam = 0;

    if (instrParamStr[0] == '0' && instrParamStr[1] == 'x') {
        instrParam = std::stoi(instrParamStr.substr(2), nullptr, 16);
    }
    else if (instrParamStr[0] == '0' && instrParamStr[1] == 'b') {
        instrParam = std::stoi(instrParamStr.substr(2), nullptr, 2);
    }
    else if (IsStrAlpha(instrParamStr)) {
        auto it = instrParamConv.find(instrParamStr);

        if (it != instrParamConv.end()) { // Contains Key
            instrParam = instrParamConv[instrParamStr];
        }
        else {
            instrParam = assembleState.labels[instrParamStr];
        }
    }
    else {
        instrParam = std::stoi(instrParamStr);
    }
    return instrParam;
}

int ConvertLineInstruction(AssembleState& assembleState, const std::vector<std::string>& line) {
    if (line[0] == "%") {
        if (line[1] == "org") {
            if (line.size() > 1) {
                assembleState.currentAddress = ConvertInstrParam(assembleState, line[2]);
            }
        }
        else if (line[1] == "dw") {
            while (assembleState.binProgram.size() <= assembleState.currentAddress) {
                assembleState.binProgram.push_back(0);
            }
            if (line.size() > 1) {
                uint16_t instrParam = ConvertInstrParam(assembleState, line[2]);

                assembleState.binProgram[assembleState.currentAddress] = (instrParam & 0xff00) >> 8;
                assembleState.binProgram.push_back(instrParam & 0x00ff);
            }
            else {
                assembleState.binProgram[assembleState.currentAddress] = 0;
                assembleState.binProgram.push_back(0);
            }
            return 2;
        }
        else if (line[1] == "db") {
            while (assembleState.binProgram.size() <= assembleState.currentAddress) {
                assembleState.binProgram.push_back(0);
            }
            if (line.size() > 1) {
                assembleState.binProgram[assembleState.currentAddress] = ConvertInstrParam(assembleState, line[2]);
            }
            else {
                assembleState.binProgram[assembleState.currentAddress] = 0;
            }
            return 1;
        }
        return 0;
    }

    std::string instrToken = line[0];

    uint8_t instr;
    auto it = instructions.find(instrToken);

    if (it != instructions.end()) { // Contains Key
        instr = instructions[instrToken].index;
    }
    else {
        printf("Error: Invalid Instruction Opcode: %s\n", instrToken.c_str());
        return 0;
    }
    uint16_t instrParam = 0;

    uint16_t extraParam = 0;
    bool extraParamOn = false;

    if (line.size() - 1 != instructions[instrToken].parameters.size() || instr == MOV) { // For Non Imm Versions or Imm Versions like LDWR or MOVI
        switch (instr) {
        case LDW:
        case LDB:
        case STW:
        case STB:
        case LDXW:
        case LDXB:
        case STXW:
        case STXB: {
            instr++;
            instrToken = "ldw+";

            break;
        }
        case MOV: {
            std::string immParamToken = line[2];

            if (immParamToken[0] == '0' && immParamToken[1] == 'x') {
                instr++;
                instrToken = "movi";
            }
            else if (immParamToken[0] == '0' && immParamToken[1] == 'b') {
                instr++;
                instrToken = "movi";
            }
            else if (!IsStrAlpha(immParamToken)) {
                auto it = instrParamConv.find(immParamToken);

                if (it == instrParamConv.end()) { // Does Not Contain Key
                    instr++;
                    instrToken = "movi";
                }
            }

            break;
        }
        default: {
            printf("Error: Less Tokens than Required for Instruction: %s\n", line[0].c_str());
            return 0;
        }
        }
    }

    for (int i = 0; i < instructions[instrToken].parameters.size(); i++) {
        if (instructions[instrToken].parameters[i] == ParameterIndex::OP0) {
            instrParam |= ((ConvertInstrParam(assembleState, line[i + 1]) & 0b11) - 1 << 8);
        }
        else if (instructions[instrToken].parameters[i] == ParameterIndex::OP1) {
            instrParam |= ((ConvertInstrParam(assembleState, line[i + 1]) & 0b1111) << 4);
        }
        else if (instructions[instrToken].parameters[i] == ParameterIndex::OP2) {
            instrParam |= (ConvertInstrParam(assembleState, line[i + 1]) & 0b1111);
        }
        else if (instructions[instrToken].parameters[i] == ParameterIndex::IMM) {
            extraParam = ConvertInstrParam(assembleState, line[i + 1]);
            extraParamOn = true;
        }
    }

    while (assembleState.binProgram.size() <= assembleState.currentAddress) {
        assembleState.binProgram.push_back(0);
    }

    uint16_t instrWord = 0xFC00 & (instr << 10) | 0x03FF & (instrParam);
    assembleState.binProgram[assembleState.currentAddress] = (instrWord & 0xff00) >> 8;
    assembleState.binProgram.push_back(instrWord & 0x00ff);

    if (extraParamOn) {
        assembleState.binProgram.push_back((extraParam & 0xff00) >> 8);
        assembleState.binProgram.push_back(extraParam & 0x00ff);
        return 4;
    }
    return 2;
}