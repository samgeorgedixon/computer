#include "entry.h"

#include <fstream>

#include "core.h"

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

void WriteASMFile(std::vector<std::string>& asmProgram, std::string asmFileName) {
    // ASM File
    std::ofstream asmFile(asmFileName);

    std::ostream_iterator<std::string> asmFileIterator(asmFile, "\n");
    std::copy(std::begin(asmProgram), std::end(asmProgram), asmFileIterator);

    asmFile.close();
}

void WriteBINFile(std::vector<char>& binProgram, std::string binFileName) {
    std::ofstream outFile(binFileName, std::ios::out | std::ios::binary);

    outFile.write(&binProgram[0], binProgram.size() * sizeof(char));

    outFile.close();
}
