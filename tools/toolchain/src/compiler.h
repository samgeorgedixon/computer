#pragma once
#include <string>
#include <vector>

#include "entry.h"

std::vector<std::vector<std::string>> LoadFile(std::string filePath);
std::vector<std::string> CreateProgram(std::vector<std::vector<std::string>>& lines, Args& args);