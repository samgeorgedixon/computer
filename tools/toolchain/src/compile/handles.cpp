#include "handles.h"

#include <string>
#include <vector>

#include "core/parse.h"

#include "compile.h"
#include "get.h"
#include "conversion_c.h"
#include "variable.h"

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
        printf("C Error: Missing Opening Scope on While Loop");
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
        printf("C Error: Invalid Brackets Usage on Assembly Directive");
        return;
    }
    std::vector<std::vector<std::string>> parameters = GetParameters(line);

	if (parameters.size() != 1 || parameters[0][0] != "\"" || parameters[0][parameters[0].size() - 1] != "\"") {
		printf("C Error: Invalid Parameter Usage on Assembly Directive");
		return;
    }
	std::vector<std::string> asmLine(parameters[0].begin() + 1, parameters[0].end() - 1);

    WRITE_LINE_ASM_PROGRAM(asmLine);
}

void HandleFunctionCall(CompileState_C& compileState, const std::vector<std::string>& line) {

}

void HandleFunctionDeclaration(CompileState_C& compileState, const std::vector<std::string>& line) {

}

void HandleVariableAssignment(CompileState_C& compileState, const std::vector<std::string>& line) {
    Type type = NONE_TYPE;

    bool declaration = false;
    bool isStack = true;

    bool isArray = false;
    int operatorIndex = 1; // Default: [0 = var, 1 = operator]
    
    std::string variableName = "";
    int arrayCount = 0;

    auto itType = types.find(line[0]);
    if (itType != types.end()) { // Contains Key
        type = types[line[0]];
        declaration = true;
    }
    variableName = line[0 + declaration];

    // Get Array Count
    if (line.size() > 1 + declaration) {
        if (line[1 + declaration] == "[") {
            isArray = true;

            std::vector<std::string> arrayCentre = GetArrayCentre(line, 2 + declaration);
            operatorIndex += arrayCentre.size() + 2;

            std::vector<std::vector<std::string>> arrayExpression = GetExpression(arrayCentre);
			PotentialImmediate arrayPotentialImmediate = ConvertExpression(compileState, arrayExpression, true, "r3", false); // TODO: Not sure if last argument is correct

            if (arrayPotentialImmediate.isImmediate == true) {
                arrayCount = arrayPotentialImmediate.immediate;
			}
			else { // Not Immediate
				// arrayCount = notImmediateValue...; // TODO: What about notImmediate though... Variable
			}
        }
    }

    if (declaration) {
        operatorIndex++;

        if (arrayCount == 0) {
            arrayCount = 1;
        }
        
        if (compileState.scopes.size() == 0) {
            DeclareDataVariable(compileState, variableName, type, arrayCount);
            isStack = false;
        }
        else {
            DeclareStackVariable(compileState, variableName, type, arrayCount);
        }
    }

	if (line.size() <= 1 + declaration || (isArray && line.size() <= operatorIndex - 1)) { // No Assignment
        printf("C Warning: No Assignment for Variable: %s\n", variableName.c_str());
        return;
    }

	// Get Operator
    auto itOp = expressionOperators.find(line[operatorIndex]);

    ExpressionOperator expressionOperator = NONE_OP;
    if (itOp != expressionOperators.end()) { // Contains Key
        expressionOperator = expressionOperators[line[operatorIndex]];
    }
    else { // No Operator
        printf("C Error: Invalid Operator for Variable Assignment: %s, %s\n", line[operatorIndex].c_str(), variableName.c_str());
        return;
    }

    //
	// Variable Assignment
    //

    if (declaration) {
        arrayCount = 0; // Now an Array Index
    }

    std::vector<std::string> lineExpression(line.begin() + operatorIndex + 1, line.end());

    std::vector<std::vector<std::string>> expression = GetExpression(lineExpression);

    if (expression.size() == 0) {
        return;
    }
    else if (expression.size() > 1) {
        ConvertExpression(compileState, expression, false, "r1", false);

        LoadFromORStoreToVariable(compileState, false, variableName, arrayCount, "r1");

        return;
    }

	// 1: Expression Part
    std::vector<std::string> expressionPart = expression[0];

    int expressionArrayIndex = 0;
    if (expressionPart.size() > 1) {
        if (expressionPart[1] == "(" && expressionPart[expressionPart.size() - 1] == ")") { // Function Expression
            std::vector<std::string> functionLine = { "r1", "=" };

            functionLine.insert(functionLine.end(), expressionPart.begin(), expressionPart.end());

            HandleFunctionCall(compileState, functionLine); // TODO: Ensure Function Returns to "r1"

            return;
        }
        else if (expressionPart[1] == "[" && expressionPart[expressionPart.size() - 1] == "]") { // Variable Array Index Expression
            std::vector<std::string> arrayCentre = GetArrayCentre(line, operatorIndex + 3);

            std::vector<std::vector<std::string>> arrayExpression = GetExpression(arrayCentre);
            PotentialImmediate arrayPotentialImmediate = ConvertExpression(compileState, arrayExpression, true, "r3", false);

            if (arrayPotentialImmediate.isImmediate == true) {
                expressionArrayIndex = arrayPotentialImmediate.immediate;
            }
            else { // Not Immediate
                // arrayCount = notImmediateValue...; // TODO: What about notImmediate though... Expression
            }
        }
    }

    LoadFromORStoreToVariable(compileState, true, expressionPart[0], expressionArrayIndex, "r2");

    switch (expressionOperator) {
        default: break;
        case SET_ADD: {
            LoadFromORStoreToVariable(compileState, true, variableName, arrayCount, "r1");
            
            WRITE_LINE_ASM_PROGRAM("add", "r1", "r1", "r2");
            break;
        }
        case SET_MINUS: {
            LoadFromORStoreToVariable(compileState, true, variableName, arrayCount, "r1");

            WRITE_LINE_ASM_PROGRAM("sub", "r1", "r1", "r2");
            break;
        }
    }

    LoadFromORStoreToVariable(compileState, false, variableName, arrayCount, "r1");

    return;
}
