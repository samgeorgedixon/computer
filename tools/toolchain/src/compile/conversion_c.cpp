#include "conversion_c.h"

#include "core/core.h"
#include "compile.h"
#include "handles.h"
#include "get.h"

void ConvertUnknownLine(CompileState_C& compileState, const std::vector<std::string>& line) {
    if (line[1] == "(") {
        HandleFunctionCall(compileState, line);
    }
    else if (line[2] == "(") {
        HandleFunctionDeclaration(compileState, line);
    }
    else {
        HandleVariableAssignment(compileState, line);
    }
}

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
        ConvertUnknownLine(compileState, line);
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

PotentialImmediate ConvertPotentialImmediate(std::string immediate) {
    PotentialImmediate potImm = { 0, false };
    
    if (immediate.size() > 1) {
        if (immediate[0] == '0' && immediate[1] == 'x') {
            potImm.isImmediate = true;
            potImm.immediate = std::stoi(immediate.substr(2), nullptr, 16);
        }
        else if (immediate[0] == '0' && immediate[1] == 'b') {
            potImm.isImmediate = true;
            potImm.immediate = std::stoi(immediate.substr(2), nullptr, 2);
        }
        else if (!IsStrAlpha(immediate)) {
            potImm.isImmediate = true;
            potImm.immediate = std::stoi(immediate);
        }
        else {
            return potImm;
        }
    }
    else if (!IsStrAlpha(immediate)) {
        potImm.isImmediate = true;
        potImm.immediate = std::stoi(immediate);
    }
    else {
        return potImm;
    }
    return potImm;
}

PotentialImmediate SolveExpressionPart(CompileState_C& compileState, const std::vector<std::string>& expressionPart, std::string desiredRegister) {
    std::vector<std::string> line = {};

    PotentialImmediate result = { 0, false };

    if (expressionPart.size() == 0) {
        return { 0, true };
    }
	else if (expressionPart.size() == 1) {
        result = ConvertPotentialImmediate(expressionPart[0]);

        if (result.isImmediate) {
            return result;
        }
    }

    // Not Immediate
    
    if (expressionPart[0] == "(" && expressionPart[expressionPart.size() - 1] == ")") {
        line.insert(line.end(), expressionPart.begin() + 1, expressionPart.end() - 1);

        std::vector<std::vector<std::string>> subExpression = GetExpression(expressionPart);
        return ConvertExpression(compileState, subExpression, true, desiredRegister, true); // // TODO: Not sure if last argument is correct
    }
    
    line.push_back(desiredRegister);
    line.push_back("=");

    line.insert(line.end(), expressionPart.begin(), expressionPart.end());

    ConvertUnknownLine(compileState, line);

    return result;
}

PotentialImmediate SolveOperator(CompileState_C& compileState, ExpressionOperator expressionOperator, const PotentialImmediate& partA, const PotentialImmediate& partB, std::string desiredRegister) {
    PotentialImmediate result = { 0, true };
    
    if (partA.isImmediate == true && partB.isImmediate == true) {
        switch (expressionOperator) {
            default:
            case SET_EQUAL:
            case NONE_OP: {
                printf("C Error: Invalid Operator: %s\n", expressionOperator);
                break;
            }
            case EQUAL: { break; }
            case NOT_EQUAL: { break; }
            case LESS: { break; }
            case GREATER: { break; }
            case LESS_EQUAL: { break; }
            case GREATER_EQUAL: { break; }
            case ADD: { result.immediate = partA.immediate + partB.immediate; break; }
            case MINUS: { result.immediate = partA.immediate - partB.immediate; break; }
        }
    }

    if (partA.isImmediate == true) { // Only Either Or a/b Now: Not Both
        WRITE_LINE_ASM_PROGRAM("mov", "r1", std::to_string(partA.immediate));
    }
    else if (partB.isImmediate == true) {
        WRITE_LINE_ASM_PROGRAM("mov", "r2", std::to_string(partB.immediate));
    }

    switch (expressionOperator) {
        default:{
            printf("C Error: Invalid Operator: %s\n", expressionOperator);
            break;
        }
        case EQUAL:         { WRITE_LINE_ASM_PROGRAM("cmp", "r1", "r2"); break; }
        case NOT_EQUAL:     { break; }
        case LESS:          { break; }
        case GREATER:       { break; }
        case LESS_EQUAL:    { break; }
        case GREATER_EQUAL: { break; }
        case ADD:           { WRITE_LINE_ASM_PROGRAM("add" "r1", "r1", "r2"); break; }
        case MINUS:         { WRITE_LINE_ASM_PROGRAM("sub" "r1", "r1", "r2"); break; }
    }

    return result;
}

PotentialImmediate ConvertExpression(CompileState_C& compileState, const std::vector<std::vector<std::string>>& expression, bool desiredIsImmediate, std::string desiredRegister, bool subExpression) {
	bool savedRegisters = false;
    
    // TODO: Push then pop: r1-2 (to save them) 
    // TODO: Check Not Overwrighting Registers

    PotentialImmediate result = { 0, true };

    for (int i = 0; i < expression.size(); i += 2) { // Miss Out Operators
        PotentialImmediate currentResult = { 0, true };

        if (i == 0) {
            currentResult = SolveExpressionPart(compileState, expression[i], "r1");
        }
        else {
            currentResult = SolveExpressionPart(compileState, expression[i], "r2");

            auto it = expressionOperators.find(expression[i - 1][0]);
            if (it == expressionOperators.end()) { // Does Not Contain Key
                printf("C Error: Invalid Operator: %s\n", expression[i - 1][0].c_str());
                break;
            }
            
            ExpressionOperator expressionOperator = expressionOperators[expression[i - 1][0]];
            
			std::string currentResultRegister = (i != expression.size() - 1) ? "r1" : desiredRegister;
            currentResult = SolveOperator(compileState, expressionOperator, result, currentResult, currentResultRegister);
        }

        result = currentResult;
    }

    if (result.isImmediate == true && desiredIsImmediate == false) {
        WRITE_LINE_ASM_PROGRAM("mov", desiredRegister, std::to_string(result.immediate));
		result.isImmediate = false;
    }
	else if (result.isImmediate == false && desiredIsImmediate == true) {
		result.isImmediate = false;
	}
    
    return result;
}
