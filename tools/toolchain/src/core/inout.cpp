#include "inout.h"

#include <fstream>
#include <vector>
#include <string>
#include <bitset>

#include "core.h"

void PrintASMProgram(const std::vector<std::vector<std::string>> asmProgram) {
    printf("---\n");
    for (int i = 0; i < asmProgram.size(); i++) {
        printf("%i: ", i);
        for (int j = 0; j < asmProgram[i].size(); j++) {
            printf("%s ", asmProgram[i][j].c_str());
        }
        printf("\n");
    }
    printf("---\n");
}

void PrintBINProgram(const std::vector<char> binProgram) {
    printf("---\n");

    int skipped = 0;

    for (int i = 0; i < binProgram.size(); i += 2) {
        if (i >= 2) {
            if (binProgram[i - 2] == 0 && binProgram[i - 1] == 0 && binProgram[i] == 0 && binProgram[i + 1] == 0) {
                skipped++;
                continue;
            }
        }
        if (skipped != 0) {
            printf("* - %i\n", skipped * 2);
            skipped = 0;
        }

        printf("%i: %s - %i: %s\n", i, std::bitset<8>(binProgram[i]).to_string().c_str(), i + 1, std::bitset<8>(binProgram[i + 1]).to_string().c_str());
    }
    printf("Bytes: %i\n", binProgram.size());
}

void WriteASMFile(const std::vector<std::vector<std::string>>& asmProgram, const std::string asmFileName) {
    std::ofstream asmFile(asmFileName);

    for (const auto& line : asmProgram) {
        for (const auto& token : line) {
            asmFile << token << " ";
        }
        asmFile << "\n";
    }

    asmFile.close();
}

void WriteBINFile(const std::vector<char>& binProgram, const std::string binFileName) {
    std::ofstream outFile(binFileName, std::ios::out | std::ios::binary);

    outFile.write(&binProgram[0], binProgram.size() * sizeof(char));

    outFile.close();
}

Args GetPaths(int argc, char* argv[]) {
    Args args = { ToolchainType::ASSEMBLE, "", "", "", false, false, ExeType::NONE, 0 };

    for (int i = 1; i < argc; i++) {
        std::string arg = Trim(argv[i]);

        if (arg == "" || arg == "help") {
            printf("Usage: toolchain [type] [options] src\n");
            printf("Type:\n");
            printf("  assemble: Assemble src.asm file.\n");
            printf("  compile-c: Compile src.c file.\n");
            printf("Options:\n");
            printf("  -o bin_file: Output Binary File Name.\n");
            printf("  -a asm_file: Output Assembly File Name.\n");
            printf("  -e exe_type: Executable Type (ofst_data + data_seg_offset or size_seg).\n");
            continue;
        }
        if (arg == "assemble") {
            args.toolchainType = ToolchainType::ASSEMBLE;
        }
        else if (arg == "compile-c") {
            args.toolchainType = ToolchainType::COMPILE_C;
        }
        else if (arg[0] == '-' && arg[1] == 'o') {
            args.binFile = arg;
            args.binFileSet = true;
            i++;
        }
        else if ((arg[0] == '-' && arg[1] == 'a')) {
            args.asmFile = arg;
            args.asmFileSet = true;
            i++;
        }
        else if ((arg[0] == '-' && arg[1] == 'e')) {
            i++;
            if (arg == "ofst_data") {
                args.exeType = ExeType::OFST_DATA;
                i++;
                args.dataSegOffset = std::stoi(arg);
            }
            else if (arg == "size_seg") {
                args.exeType = ExeType::SIZE_SEG;
            }
        }
        else if (arg != "") {
            args.src = arg;
        }
    }

    return args;
}
