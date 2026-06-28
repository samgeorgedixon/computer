#pragma once
#include <vector>

#include "core/inout.h"
#include "core/parse.h"

#include "assemble/assemble.h"
#include "compile/compile.h"

struct ToolchainResult {
	std::vector<std::vector<std::string>> asmProgram;
	std::vector<char> binProgram;
};

ToolchainResult Assemble(const Args& args);

ToolchainResult CompileC(const Args& args);

ToolchainResult RunToolchain(const Args& args);
