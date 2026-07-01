#pragma once
#include <string>
#include <vector>

struct Condition {
	std::vector<std::string> left;
	std::string op;
	std::vector<std::string> right;
};

Condition GetCondition(const std::vector<std::string>& bracketedList);

std::vector<std::vector<std::string>> GetParameters(const std::vector<std::string>& bracketedList);

std::vector<std::vector<std::string>> GetExpression(const std::vector<std::string>& list);

std::vector<std::string> GetArrayCentre(const std::vector<std::string>& line, int arrayStartIndex);
