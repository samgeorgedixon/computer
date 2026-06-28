#include "toolchain.h"

int main(int argc, char* argv[]) {
    Args args = GetPaths(argc, argv);

#ifdef DEBUG
	args.toolchainType = ToolchainType::COMPILE_C;

    args.src = "../../software/tests/test.c";
    args.asmFile = "../../software/tests/test.asm";
	args.asmFileSet = true;
#endif
    
    ToolchainResult result = RunToolchain(args);

    PrintASMProgram(result.asmProgram);
    PrintBINProgram(result.binProgram);

    return 0;
}
