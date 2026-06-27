#pragma once
#include <vector>
#include <string>
#include <unordered_map>

struct AssembleData {
    std::vector<char> binProgram;
        
    std::unordered_map<std::string, unsigned int> labels;
    
    int currentAddress = 0;
};

std::vector<char> AssembleLines(std::vector<std::vector<std::string>>& lines);
