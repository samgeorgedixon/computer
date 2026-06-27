#include "assembler.h"

#include <fstream>
#include <vector>
#include <iostream>

#include "core.h"

std::string srcPath;
std::string outPath;

void GetPaths(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
		std::string arg = Trim(argv[i]);
        
        if (arg[0] == '-' && arg[1] == 'o') {
            i++;
            outPath = arg;
        } else {
            srcPath = arg;
        }
    }
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
