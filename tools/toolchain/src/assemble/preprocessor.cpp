#include "preprocessor.h"

#include <unordered_map>

#include "core/core.h"
#include "core/parse.h"
#include "assemble.h"
#include "conversion_asm.h"

int CheckLineSize(AssembleState& assembleState, std::vector<std::string> line) {
    if (line[0] == "%") {
        if (line[1] == "org") {
            if (line.size() > 2) {
                assembleState.currentAddress = ConvertInstrParam(assembleState, line[2]);
            }
        }
        else if (line[1] == "dw") {
            return 2;
        }
        else if (line[1] == "db") {
            return 1;
        }
        else if (line[1] == "segment") {
            assembleState.currentAddress = 0;
        }
        return 0;
    }
    else if (line[0] == ":") {
        assembleState.labels[line[1]] = assembleState.currentAddress;
        return 0;
    }

    auto it = instructions.find(line[0]);

    if (it != instructions.end()) { // Contains Key
        if (line.size() - 1 != instructions[line[0]].parameters.size() || instructions[line[0]].index == MOV) { // For Non Imm Versions or Imm Versions like LDWR or MOVI
            switch (instructions[line[0]].index) {
            case LDW:
            case LDB:
            case STW:
            case STB:
            case LDXW:
            case LDXB:
            case STXW:
            case STXB: {
                return 2;
            }
            case MOV: {
                std::string immParamToken = line[2];

                if (immParamToken[0] == '0' && immParamToken[1] == 'x') {
                    return 4;
                }
                else if (immParamToken[0] == '0' && immParamToken[1] == 'b') {
                    return 4;
                }
                else if (!IsStrAlpha(immParamToken)) {
                    auto it = instrParamConv.find(immParamToken);

                    if (it == instrParamConv.end()) { // Does Not Contain Key
                        return 4;
                    }
                }

                break;
            }
            default: {
                printf("Error: Less Tokens than Required for Instruction: %s, %s\n", line[0], line);
                return 0;
            }
            }
        }

        return instructions[line[0]].byteSize;
    }
    else {
        printf("Error: Invalid Instruction Opcode: %s, %s\n", line[0], line);
        return 0;
    }
}

void SetLabels(AssembleState& assembleState, const std::vector<std::vector<std::string>>& lines) {
    for (int i = 0; i < lines.size(); i++) {
        assembleState.currentAddress += CheckLineSize(assembleState, lines[i]);
    }
}

void RunPreprocessor(AssembleState& assembleState, std::vector<std::vector<std::string>>& lines) {
    std::unordered_map<std::string, std::vector<std::string>> defines;

    for (int i = 0; i < lines.size(); i++) {
        std::vector<std::string> line = lines[i];
        std::string str;

        if (line[0] == "%") {
            if (line[1] == "define") {
                std::vector<std::string> define(line.begin() + 3, line.end());

                defines[line[2]] = line;
            }
            else if (line[1] == "include") {
                std::string includePath = line[3];

                std::vector<std::vector<std::string>> asmFile = ParseASMFile(includePath);
                lines.insert(lines.begin() + i, asmFile.begin(), asmFile.end());
            }
        }

        for (int j = 0; j < line.size(); j++) {
            if (line[j] == "$") {
                std::vector<std::string> define = defines[line[j + 1]];

                line.insert(line.begin() + j + 2, define.begin(), define.end());

                line.erase(line.begin() + j, line.begin() + j + 2);
            }
        }
    }
    SetLabels(assembleState, lines);
}
