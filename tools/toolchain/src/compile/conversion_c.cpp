#include "conversion_c.h"

#include "compile.h"
#include "handles.h"

void ConvertLine(CompileState_C& compileState, int lineIndex) {
    std::vector<std::string>& line = compileState.lines[lineIndex];

    auto it = baseLineIdentifiers.find(line[0]);

    BaseLineIdentifier baseLineIdentifier = UNKNOWN;
    if (it != baseLineIdentifiers.end()) { // Contains Key
        baseLineIdentifier = baseLineIdentifiers[line[0]];
    }

    switch (baseLineIdentifier) {
    case PREPROCESSOR: {
        HandlePreprocessorDirective(compileState, line, lineIndex);
        break;
    }
    case WHILE: {
        HandleWhile(compileState, line, lineIndex);
        break;
    }
    case FOR: {
        HandleFor(compileState, line);
        break;
    }
    case IF: {
        HandleIf(compileState, line);
        break;
    }
    case ELSE: {
        if (line[1] == "if") {
            HandleElseIf(compileState, line);
        }
        else {
            HandleElse(compileState, line);
        }
        break;
    }
    case SCOPE_OPEN: {
        HandleScopeOpen(compileState, line);
        break;
    }
    case SCOPE_CLOSE: {
        HandleScopeClose(compileState, line);
        break;
    }
    case RETURN: {
        HandleReturn(compileState, line);
        break;
    }
    case ASM: {
        HandleAssemblyDirective(compileState, line);
        break;
    }
    case UNKNOWN: // Variables, Functions
    default: {
        if (line[1] == "(") {
            HandleFunctionCall(compileState, line);
        }
        else if (line[2] == "(") {
            HandleFunctionDeclaration(compileState, line);
        }
        else {
            HandleVariableAssignment(compileState, line);
        }
        break;
    }
    }
}

void ConvertLines(CompileState_C& compileState) {
    for (int i = 0; i < compileState.lines.size(); i++) {
        // TODO: Preprocessor Check

        ConvertLine(compileState, i);
    }
}

int ConvertExpression(const std::vector<std::vector<std::string>>& expression) {
    return 0;
}
