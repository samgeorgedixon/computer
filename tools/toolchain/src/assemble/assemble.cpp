#include "assemble.h"

#include <vector>
#include <string>

#include "core/core.h"
#include "core/parse.h"
#include "core/inout.h"

#include "preprocessor.h"
#include "conversion_asm.h"

void CreateBINProgram(AssembleState& assembleState, const std::vector<std::vector<std::string>>& lines) {
    assembleState.currentAddress = 0;

    for (int i = 0; i < lines.size(); i++) {
        if (lines[i][0] == ":") {
            continue;
        }
        assembleState.currentAddress += ConvertLineInstruction(assembleState, lines[i]);
    }
}

std::vector<char> AssembleLines(std::vector<std::vector<std::string>>& lines) {
	AssembleState assembleState;

    RunPreprocessor(assembleState, lines);
    CreateBINProgram(assembleState, lines);

    return assembleState.binProgram;
}
