#include "get.h"

#include <string>
#include <unordered_map>

enum ConditionOperator {
    NONE_CONDITION = 0, EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL
};

std::unordered_map<std::string, ConditionOperator> conditionOperators = {
    // none
    { "==",          EQUAL },
    { "!=",          NOT_EQUAL },
    { "<",           LESS },
    { ">",           GREATER },
    { "<=",          LESS_EQUAL },
    { ">=",          GREATER_EQUAL }
};

enum MathsOperator {
    NONE_MATHS_OP = 0, SET_EQUAL, ADD, MINUS, MULTIPLY, DIVIDE
};

std::unordered_map<std::string, MathsOperator> mathsOperators = {
    // none
    { "=",          SET_EQUAL },
    { "+",          ADD },
    { "-",          MINUS },
    { "*",          MULTIPLY },
    { "/",          DIVIDE },
};

Condition GetCondition(const std::vector<std::string>& bracketedList) {
	Condition condition;

	for (int i = 0; i < bracketedList.size(); i++) {
        auto it = conditionOperators.find(bracketedList[i]);

		bool isLeft = true;
        
        ConditionOperator conditionOperator = NONE_CONDITION;
        if (it != conditionOperators.end()) { // Contains Key
            conditionOperator = conditionOperators[bracketedList[i]];
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

std::vector<std::vector<std::string>> GetExpression(const std::vector<std::string>& list) { // TODO: Brackets / BIDMAS
    std::vector<std::vector<std::string>> expression;

    int expressionIndex = 0;

    for (int i = 0; i < list.size(); i++) {
        auto it = conditionOperators.find(list[i]);

        //MathsOperator mathsOperator = NONE;
        if (it != conditionOperators.end()) { // Contains Key
            //mathsOperator = mathsOperators[list[i]];
            
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

std::string GetParamaeterValueORImmediate(const std::vector<std::string>& parameter) {
    return "";
}
