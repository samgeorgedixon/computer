#pragma once
#include <string>

enum ExeType {
    NONE = 0,
    OFST_DATA,  // Offsets Data segment by segments.
    SIZE_SEG    // Include sizes of segments at start.
};

struct Args {
    std::string src;
    std::string out;
    std::string asmb;

    ExeType exeType;
    int dataSegOffset;
};

Args GetPaths(int argc, char* argv[]);
