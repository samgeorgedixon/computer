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

Args GetPaths(int argc, char* argv[]);

void WriteASMFile(std::vector<std::string>& asmProgram, std::string asmFileName);

void WriteBINFile(std::vector<char>& binProgram, std::string binFileName);
