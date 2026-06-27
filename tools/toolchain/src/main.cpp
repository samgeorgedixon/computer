#include "toolchain.h"

int main(int argc, char* argv[]) {
    Args args = GetPaths(argc, argv);

#ifdef DEBUG
	args.toolchainType = ToolchainType::ASSEMBLE;

    args.src = "../../software/tests/gpu_test.asm";
    args.binFile = "../../bin/software/gpu_test.bin";
	args.binFileSet = true;
#endif
    
    ToolchainResult result = RunToolchain(args);

    PrintASMProgram(result.asmProgram);
    PrintBINProgram(result.binProgram);

    return 0;
}
