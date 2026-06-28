#pragma once
#include <string>
#include <vector>

#include "compile.h"

void HandlePreprocessorDirective(CompileState_C& compileState, const std::vector<std::string>& line, int lineIndex);

void HandleWhile				(CompileState_C& compileState, const std::vector<std::string>& line, int lineIndex);
void HandleFor					(CompileState_C& compileState, const std::vector<std::string>& line);

void HandleIf					(CompileState_C& compileState, const std::vector<std::string>& line);
void HandleElseIf				(CompileState_C& compileState, const std::vector<std::string>& line);
void HandleElse					(CompileState_C& compileState, const std::vector<std::string>& line);

void HandleScopeOpen			(CompileState_C& compileState, const std::vector<std::string>& line);
void HandleScopeClose			(CompileState_C& compileState, const std::vector<std::string>& line);

void HandleReturn				(CompileState_C& compileState, const std::vector<std::string>& line);
void HandleAssemblyDirective	(CompileState_C& compileState, const std::vector<std::string>& line);

void HandleFunctionCall			(CompileState_C& compileState, const std::vector<std::string>& line);
void HandleFunctionDeclaration	(CompileState_C& compileState, const std::vector<std::string>& line);

void HandleVariableAssignment	(CompileState_C& compileState, const std::vector<std::string>& line);
