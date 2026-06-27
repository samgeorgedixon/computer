#pragma once
#include <string>
#include <vector>

enum ExeType {
    NONE = 0,
    OFST_DATA,  // Offsets Data segment by segments.
    SIZE_SEG    // Include sizes of segments at start.
};

enum ToolchainType {
    ASSEMBLE = 0,
    COMPILE_C
};

struct Args {
    ToolchainType toolchainType;

    std::string src;

    std::string binFile;
    std::string asmFile;

    bool binFileSet;
    bool asmFileSet;

    ExeType exeType;
    int dataSegOffset;
};


void PrintASMProgram(const std::vector<std::vector<std::string>> asmProgram);
void PrintBINProgram(const std::vector<char> binProgram);

void WriteASMFile(const std::vector<std::vector<std::string>>& asmProgram, std::string asmFileName);
void WriteBINFile(const std::vector<char>& binProgram, const std::string binFileName);

Args GetPaths(int argc, char* argv[]);
