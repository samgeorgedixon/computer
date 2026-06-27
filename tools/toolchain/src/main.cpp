#include "core/entry.h"
#include "core/parser.h"

#include "assembler.h"
//#include "compiler.h"

int main(int argc, char* argv[]) {
    Args args = GetPaths(argc, argv);

#ifdef DEBUG
	args.toolchainType = ToolchainType::ASSEMBLE;

    args.src = "../../software/tests/gpu_test.asm";
    args.binFile = "../../bin/software/gpu_test.bin";
	args.binFileSet = true;
#endif
    
    std::vector<std::vector<std::string>> lines;

    if (args.toolchainType == ToolchainType::ASSEMBLE) {
		lines = ParseASMFile(args.src);

        if (lines.size() == 0) {
            return 1;
        }
        
        std::vector<char> binProgram = AssembleLines(lines);
        if (args.binFileSet) { WriteBINFile(binProgram, args.binFile); }
	}
    else if (args.toolchainType == ToolchainType::COMPILE_C) {
        //lines = ParseASMFile(args.src);
        ////std::vector<std::vector<std::string>> lines = LoadFile(args.src);
        //
        //if (lines.size() == 0) {
        //    return 1;
        //}
        //
        //std::vector<std::vector<std::string>> asmProgram = CreateProgram(lines, args);
        //std::vector<char> binProgram = AssembleLines(asmProgram);
        //
        //WriteASMFile(args, asmProgram);
        //WriteBINFile(args, binProgram);
    }

    return 0;
}