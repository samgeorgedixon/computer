#include "handles.h"

#include <string>
#include <vector>

#include "core/parse.h"

#include "compile.h"
#include "get.h"

void HandlePreprocessorDirective(CompileState_C& compileState, const std::vector<std::string>& line, int lineIndex) {
    if (line.size() <= 2) {
        // Put .size() = 2 Here
        return;
    }
    else if (line[1] == "include") {
        std::vector<std::vector<std::string>> includeLines = ParseCFile(line[2]);

        compileState.lines.insert(compileState.lines.begin() + lineIndex + 1, includeLines.begin(), includeLines.end());
    }
}

void HandleWhile(CompileState_C& compileState, const std::vector<std::string>& line, int lineIndex) {
    std::string scopeName = "while_" + std::to_string(compileState.asmProgram.size()) + "_" + std::to_string(lineIndex);

	if (compileState.lines[lineIndex + 1][0] == "{") {
        compileState.scopes.push_back({ ScopeType::While, scopeName, compileState.relitiveStackPointer });

        WRITE_LINE_ASM_PROGRAM(":" + scopeName);

        lineIndex++;
	}
    else {
        printf("Error: Missing Opening Scope on While Loop");
        return;
    }
    
    //if (params.size() == 3) { // Check Condition
    //    if (StringToImm(params[0]).first) {
    //        LdStVar(compileState.vars[params[0]], compileState.asmProgram, "r1", "ld", 0);
    //    }
    //    else {
    //        WRITE_LINE_ASM_PROGRAM("li r1 " + std::to_string(StringToImm(params[0]).second));
    //    }
    //    if (StringToImm(params[2]).first) {
    //        LdStVar(compileState.vars[params[2]], compileState.asmProgram, "r2", "ld", 0);
    //    }
    //    else {
    //        WRITE_LINE_ASM_PROGRAM("li r2 " + std::to_string(StringToImm(params[2]).second));
    //    }

    //    WRITE_LINE_ASM_PROGRAM("cmp r1 r2");

    //    if (params[1] == "==") {
    //        WRITE_LINE_ASM_PROGRAM("jnz [z " + scopeName + "_end]");
    //    }
    //    else if (params[1] == "!=") {
    //        WRITE_LINE_ASM_PROGRAM("jz [z " + scopeName + "_end]");
    //    }
    //}
    //else { // Check Bool
    //    if (StringToImm(params[0]).first) {
    //        LdStVar(compileState.vars[params[0]], compileState.asmProgram, "r2", "ld", 0);
    //    }
    //    else {
    //        WRITE_LINE_ASM_PROGRAM("li r2 " + std::to_string(StringToImm(params[0]).second));
    //    }

    //    WRITE_LINE_ASM_PROGRAM("li r1 0");
    //    WRITE_LINE_ASM_PROGRAM("cmp r1 r2");
    //    WRITE_LINE_ASM_PROGRAM("jz [z " + scopeName + "_end]");
    //}
    WRITE_LINE_ASM_PROGRAM(":" + scopeName + "_loop");
}

void HandleFor(CompileState_C& compileState, const std::vector<std::string>& line) {

}

void HandleIf(CompileState_C& compileState, const std::vector<std::string>& line) {

}

void HandleElseIf(CompileState_C& compileState, const std::vector<std::string>& line) {

}

void HandleElse(CompileState_C& compileState, const std::vector<std::string>& line) {

}

void HandleScopeOpen(CompileState_C& compileState, const std::vector<std::string>& line) {
    std::string scopeName = "scope_" + std::to_string(compileState.asmProgram.size());

    //compileState.scopes.push_back({ , scopeName, compileState.relitiveStackPointer });

    WRITE_LINE_ASM_PROGRAM(":" + scopeName);

	// TODO: Scope Base/Relitive Stack Pointer Management
}

void HandleScopeClose(CompileState_C& compileState, const std::vector<std::string>& line) {
    //compileState.relitiveStackPointer = compileState.scopes[compileState.scopes.size() - 1].relitiveStackPointer;

    //switch (compileState.scopes[compileState.scopes.size() - 1].type)
    //{
    //case ScopeType::Func:
    //    WRITE_LINE_ASM_PROGRAM(":" + compileState.scopes[0].name + "_end");
    //    WRITE_LINE_ASM_PROGRAM("mov sp bp");
    //    WRITE_LINE_ASM_PROGRAM("pop bp");
    //    WRITE_LINE_ASM_PROGRAM("ret");

    //    if (compileState.scopes[0].name == "main") {
    //        //std::vector<std::string> main(compileState.asmProgram.begin() + compileState.mainIndex + 1, compileState.asmProgram.end());

    //        //compileState.asmProgram.erase(compileState.asmProgram.begin() + compileState.mainIndex + 1, compileState.asmProgram.end());

    //        //compileState.asmProgram.insert(compileState.asmProgram.begin() + compileState.firstFuncIndex + 1, main.begin(), main.end());
    //    }
    //    break;
    //case ScopeType::While:
    //    WRITE_LINE_ASM_PROGRAM("jmp [z " + compileState.scopes[compileState.scopes.size() - 1].name + "]");
    //    WRITE_LINE_ASM_PROGRAM(":" + compileState.scopes[compileState.scopes.size() - 1].name + "_end");

    //    WRITE_LINE_ASM_PROGRAM("li r1 " + std::to_string(compileState.scopes[compileState.scopes.size() - 1].relitiveStackPointer));
    //    WRITE_LINE_ASM_PROGRAM("add r1 bp r1");
    //    WRITE_LINE_ASM_PROGRAM("mov sp r1");
    //    break;
    //case ScopeType::If:
    //    WRITE_LINE_ASM_PROGRAM(":" + compileState.scopes[compileState.scopes.size() - 1].name + "_end");

    //    WRITE_LINE_ASM_PROGRAM("li r1 " + std::to_string(compileState.scopes[compileState.scopes.size() - 1].relitiveStackPointer));
    //    WRITE_LINE_ASM_PROGRAM("add r1 bp r1");
    //    WRITE_LINE_ASM_PROGRAM("mov sp r1");
    //    break;
    //}
    //compileState.scopes.pop_back();
}

void HandleReturn(CompileState_C& compileState, const std::vector<std::string>& line) {

}

void HandleAssemblyDirective(CompileState_C& compileState, const std::vector<std::string>& line) {
    if (line[1] != "(" || line[line.size() - 1] == ")") {
        printf("Error: Invalid Brackets Usage on Assembly Directive");
        return;
    }
    std::vector<std::vector<std::string>> parameters = GetParameters(line);

	if (parameters.size() != 1 || parameters[0][0] != "\"" || parameters[0][parameters[0].size() - 1] != "\"") {
		printf("Error: Invalid Parameter Usage on Assembly Directive");
		return;
    }
	std::vector<std::string> asmLine(parameters[0].begin() + 1, parameters[0].end() - 1);

    WRITE_LINE_ASM_PROGRAM(asmLine);
}

void HandleFunctionCall(CompileState_C& compileState, const std::vector<std::string>& line) {

}

void HandleFunctionDeclaration(CompileState_C& compileState, const std::vector<std::string>& line) {

}

void DeclareStackVariable(CompileState_C& compileState, std::string name, Type type, int arrayCount) {

}
void DeclareDataVariable(CompileState_C& compileState, std::string name, Type type, int arrayCount) {
	compileState.variables[name] = { compileState.dataSegmentPointer, type, false };

    compileState.dataSegment.push_back({ ":", name });

	if (type == INT16) {
		for (int i = 0; i < arrayCount; i++) {
		    compileState.dataSegment.push_back({ "%", "dw", "0" });
		    compileState.dataSegmentPointer += 2;
		}
	}
	else if (type == INT8) {
        for (int i = 0; i < arrayCount; i++) {
            compileState.dataSegment.push_back({ "%", "db", "0" });
            compileState.dataSegmentPointer += 1;
        }
	}
	else {
		printf("Error: Invalid Type on Variable Declaration");
	}
}

void HandleVariableAssignment(CompileState_C& compileState, const std::vector<std::string>& line) {
    auto it = types.find(line[0]);

    bool declaration = false;
    bool isStack = true;

    std::string variableName = "";
    int arrayCount = 1;

    Type type = NONE_TYPE;
    if (it != types.end()) { // Contains Key
        type = types[line[0]];

        declaration = true;
        variableName = line[1];

        // Get Array Count
        if (line.size() > 2) {
            if (line[2] == "[") {
                std::vector<std::string> arrayCentre;

                for (int i = 3; i < line.size(); i++) {
                    if (line[i] == "]") {
                        break;
                    }
                    else {
                        arrayCentre.push_back(line[i]);
                    }
                }

                std::vector<std::vector<std::string>> arrayExpresion = GetExpression(arrayCentre);
                // Get IMM / Var Count
            }
        }
        
        if (compileState.scopes.size() == 0) {
            DeclareDataVariable(compileState, variableName, type, arrayCount);
            isStack = false;
        }
        else {
            DeclareStackVariable(compileState, variableName, type, arrayCount);
        }

        if (line.size() <= 2) {
            return;
        }
    }
    else {
        variableName = line[0];

        if (line.size() <= 1) {
            return;
        }
    }

    // Assign variable to Right Side ( + Check Data Declaration as can Set IMM Straight Away)


}
