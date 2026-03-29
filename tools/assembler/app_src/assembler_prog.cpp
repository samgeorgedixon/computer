#include "assembler.h"

#include <fstream>
#include <vector>
#include <iostream>

std::string srcPath;
std::string outPath;

void GetPaths(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'o') {
            i++;
            outPath = argv[i];
        } else {
            srcPath = argv[i];
        }
    }
}

std::string Trim(std::string str, std::string whitespace = " \t\r") {
    int strBegin = str.find_first_not_of(whitespace);

    if (strBegin == std::string::npos) {
        return "";
    }

    int strEnd = str.find_last_not_of(whitespace);
    int strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

int main(int argc, char* argv[]) {
    GetPaths(argc, argv);

    std::ifstream srcFile(srcPath);

    if (!srcFile.is_open()) {
        std::cout << "Unable to open src file.\n";

        srcFile.close();
        return 1;
    }
    
    std::string line;
    std::vector<std::string> lines;

    // Get / Trim Lines
    while(getline(srcFile, line)) {
        line = Trim(line);

        if (line.empty()) {
            continue;
        }
        
        lines.push_back(line);
    }
    srcFile.close();

    std::vector<char> program = AssembleLines(lines);

    // Write Binary Program to File
    std::ofstream outFile(outPath, std::ios::out | std::ios::binary);
    outFile.write(&program[0], program.size() * sizeof(char));
    outFile.close();
    
    return 0;
}
