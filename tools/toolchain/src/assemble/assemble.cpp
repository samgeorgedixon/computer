#include "assemble.h"

#include <vector>
#include <string>

#include "core/core.h"
#include "core/parser.h"
#include "core/inout.h"

#include "preprocessor.h"
#include "conversion.h"

void CreateBINProgram(AssembleData& assembleData, const std::vector<std::vector<std::string>>& lines) {
    assembleData.currentAddress = 0;

    for (int i = 0; i < lines.size(); i++) {
        if (lines[i][0] == ":") {
            continue;
        }
        assembleData.currentAddress += ConvertLineInstruction(assembleData, lines[i]);
    }
}

std::vector<char> AssembleLines(std::vector<std::vector<std::string>>& lines) {
	AssembleData assembleData;

    RunPreprocessor(assembleData, lines);
    CreateBINProgram(assembleData, lines);

    PrintBINProgram(assembleData.binProgram);

    return assembleData.binProgram;
}
