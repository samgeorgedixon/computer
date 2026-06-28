#pragma once
#include <string>
#include <vector>

std::vector<std::vector<std::string>> ParseASMFile(std::string srcPath);

std::vector<std::vector<std::string>> ParseCFile(std::string srcPath);
