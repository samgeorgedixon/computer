#include "toolchain.h"

#include "core/inout.h"
#include "core/parse.h"

#include "assemble/assemble.h"
#include "compile/compile.h"

ToolchainResult Assemble(const Args& args) {
    std::vector<std::vector<std::string>> lines = ParseASMFile(args.src);

    if (lines.size() == 0) {
        printf("Error: No lines in src.asm\n");
        return {};
    }

    std::vector<char> binProgram = AssembleLines(lines);
    if (args.binFileSet) { WriteBINFile(binProgram, args.binFile); }
    return { lines, binProgram };
}

ToolchainResult CompileC(const Args& args) {
    std::vector<std::vector<std::string>> lines = ParseCFile(args.src);

    if (lines.size() == 0) {
        printf("Error: No lines in src.c\n");
        return {};
    }

    std::vector<std::vector<std::string>> asmProgram = CompileCLines(lines, args);
    std::vector<char> binProgram = AssembleLines(asmProgram);

    if (args.asmFileSet) { WriteASMFile(asmProgram, args.asmFile); }
    if (args.binFileSet) { WriteBINFile(binProgram, args.binFile); }

    return { asmProgram, binProgram };
}

ToolchainResult RunToolchain(const Args& args) {
	ToolchainResult result;

    if (args.toolchainType == ToolchainType::ASSEMBLE) {
        result = Assemble(args);
    }
    else if (args.toolchainType == ToolchainType::COMPILE_C) {
        result = CompileC(args);
    }

    return result;
}
