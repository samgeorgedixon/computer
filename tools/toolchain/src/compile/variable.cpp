#include "variable.h"

#include <string>

#include "core/core.h"
#include "compile.h"
#include "conversion_c.h"

void DeclareStackVariable(CompileState_C& compileState, std::string name, Type type, int arrayCount) { // TODO: Declare Stack Variables + Register Bases Array Size + Straight Initialisation
	
}
void DeclareDataVariable(CompileState_C& compileState, std::string name, Type type, int arrayCount) { // TODO: Data Variable Straight Initialisation
	compileState.variables[name] = { compileState.dataSegmentPointer, type, false };

	compileState.dataSegment.push_back({ ":", name });

	if (type == INT16) {
		for (int i = 0; i < arrayCount; i++) {
			compileState.dataSegment.push_back({ "%", "dw", "0" });
			compileState.dataSegmentPointer += 2;
		}
	}
	else if (type == INT8) {
		for (int i = 0; i < arrayCount; i++) {
			compileState.dataSegment.push_back({ "%", "db", "0" });
			compileState.dataSegmentPointer += 1;
		}
	}
	else {
		printf("C Error: Invalid Type on Variable Declaration");
	}
}

void LoadFromORStoreToVariable(CompileState_C& compileState, bool isLoadFrom, std::string variableName, int arrayIndex, std::string srcORdestRegisterName) { // Or IMM Variable
	auto itVar = compileState.variables.find(variableName);

	bool registerBased = false;
	bool immediateBased = false;
	int immediateValue = 0;
	
	VariableData variable;
	if (itVar != compileState.variables.end()) { // Contains Key
		variable = compileState.variables[variableName];
	}
	else {
		auto itReg = builtInRegisters.find(variableName);

		if (itReg != builtInRegisters.end()) { // Contains Key
			registerBased = true;
		}
		else if (isLoadFrom) {
			PotentialImmediate potImm = ConvertPotentialImmediate(variableName);

			if (potImm.isImmediate) {
				immediateBased = true;
				immediateValue = potImm.immediate;
			}
			else {
				printf("C Error: Invalid Variable Name: %s\n", variableName);
				return;
			}
		}
		else {
			printf("C Error: Invalid Variable Name: %s\n", variableName);
			return;
		}
	}

	if (immediateBased) {
		WRITE_LINE_ASM_PROGRAM("mov", srcORdestRegisterName, std::to_string(immediateValue));
		return;
	}

	if (registerBased) {
		std::string dest = srcORdestRegisterName;
		std::string src = variableName;
		if (!isLoadFrom) {
			dest = variableName;
			src = srcORdestRegisterName;
		}

		WRITE_LINE_ASM_PROGRAM("mov", dest, src);
		return;
	}

	std::string segment = "ds";
	std::string base = "z";
	int address = variable.addr + arrayIndex;
	
	if (variable.isStack) {
		segment = "ss";
		base = "bp";
		int address = -variable.addr;
	}
	
	std::string instr = "ld";
	if (!isLoadFrom) instr = "st";
	
	switch (variable.type) {
		default: { return; }
		case INT16: { instr.push_back('w'); break; }
		case INT8: { instr.push_back('b'); break; }
	}

	WRITE_LINE_ASM_PROGRAM(instr, segment, srcORdestRegisterName, base, std::to_string(address));
}
