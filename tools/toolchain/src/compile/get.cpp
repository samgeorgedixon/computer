#include "get.h"

#include <string>
#include <unordered_map>

#include "conversion_c.h"

Condition GetCondition(const std::vector<std::string>& bracketedList) {
	Condition condition;

	for (int i = 0; i < bracketedList.size(); i++) {
        auto it = expressionOperators.find(bracketedList[i]);

		bool isLeft = true;
        
        ExpressionOperator expressionOperator = NONE_OP;
        if (it != expressionOperators.end()) { // Contains Key
            expressionOperator = expressionOperators[bracketedList[i]];
        }
        else {
            if (isLeft) {
                condition.left.push_back(bracketedList[i]);
            }
            else {
                condition.right.push_back(bracketedList[i]);
            }
        }
	}
    return condition;
}

std::vector<std::vector<std::string>> GetParameters(const std::vector<std::string>& bracketedList) {
    std::vector<std::vector<std::string>> parameters;

    int parameterIndex = 0;

    for (int i = 0; i < bracketedList.size(); i++) {
        if (bracketedList[i] == ",") {
            parameterIndex++;
            parameters.push_back({});
        }
        else {
            parameters[parameterIndex].push_back(bracketedList[i]);
        }
    }
    return parameters;
}

std::vector<std::vector<std::string>> GetExpression(const std::vector<std::string>& list) { // TODO: BIDMAS
    std::vector<std::vector<std::string>> expression = {};

    expression.push_back({});

    int expressionIndex = 0;
    int insideExpressionPart = 0;
    bool insideQuotes = false;

    for (int i = 0; i < list.size(); i++) {
        if      (list[i] == "(" || list[i] == "[" || list[i] == "{") {
            insideExpressionPart++;
        }
        else if (list[i] == ")" || list[i] == "]" || list[i] == "}") {
            insideExpressionPart--;
        }
        else if (list[i] == "\"" || list[i] == "\'") {
            insideQuotes = !insideQuotes;
        }

        auto it = expressionOperators.find(list[i]);

        //operator expressionOperators = NONE_OP;
        if (it != expressionOperators.end() && !insideExpressionPart && !insideQuotes) { // Contains Key
            //operator = expressionOperators[list[i]];
            
            expressionIndex += 2;
            expression.push_back({ list[i]});
            expression.push_back({});
        }
        else {
            expression[expressionIndex].push_back(list[i]);
        }
    }
    return expression;
}

std::vector<std::string> GetArrayCentre(const std::vector<std::string>& line, int arrayStartIndex) {
    std::vector<std::string> arrayCentre;

    for (int i = arrayStartIndex; i < line.size(); i++) {
        if (line[i] == "]") {
            break;

        }
        else {
            arrayCentre.push_back(line[i]);
        }
    }

    return arrayCentre;
}

