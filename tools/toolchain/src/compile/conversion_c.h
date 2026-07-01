#pragma once
#include "compile.h"

struct PotentialImmediate {
    int immediate = 0;
    bool isImmediate = false;
};

enum ExpressionOperator {
    NONE_OP = 0, EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,
    SET_EQUAL, SET_ADD, SET_MINUS, ADD, MINUS
};

inline std::unordered_map<std::string, ExpressionOperator> expressionOperators = {
    // none
    { "==",         EQUAL },
    { "!=",         NOT_EQUAL },
    { "<",          LESS },
    { ">",          GREATER },
    { "<=",         LESS_EQUAL },
    { ">=",         GREATER_EQUAL },

    { "=",          SET_EQUAL },
    { "+=",         SET_ADD },
    { "-=",         SET_MINUS },
    { "+",          ADD },
    { "-",          MINUS },
};


void ConvertLine(CompileState_C& compileState, int lineIndex);

void ConvertLines(CompileState_C& compileState);

PotentialImmediate ConvertPotentialImmediate(std::string immediate);

PotentialImmediate ConvertExpression(CompileState_C& compileState, const std::vector<std::vector<std::string>>& expression, bool desiredIsImmediate, std::string desiredRegister, bool subExpression);
