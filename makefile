all: computer-build cpu_instr_gen-build assembler

computer: computer-build computer-run

computer-build: src/computer/computer.sv
	verilator -j 0 -O3 --x-assign fast --x-initial fast --noassert -I../ \
		--Mdir computer --trace --timing -cc --exe \
		src/computer/computer.sv src/main.cpp src/testbench/application.cpp src/testbench/computer.cpp src/load_rom_file.cpp -o computer \
		-CFLAGS "-I/usr/include/SDL2 -D_REENTRANT" -LDFLAGS "-lSDL2"

	make -C ./computer -f Vcomputer.mk

	rm -rf bin/computer
	mv computer bin
computer-run:
	bin/computer/computer

cpu_instr_gen: cpu_instr_gen-build cpu_instr_gen-run

cpu_instr_gen-build: scripts/src/cpu_instr_gen.cpp
	g++ scripts/src/cpu_instr_gen.cpp -o bin/cpu_instr_gen/cpu_instr_gen
cpu_instr_gen-run:
	bin/cpu_instr_gen/cpu_instr_gen

file-system_manager: file-system_manager-build file-system_manager-run

file-system_manager-build: scripts/src/file-system_manager.cpp
	g++ scripts/src/file-system_manager.cpp -o bin/file-system_manager/file-system_manager
file-system_manager-run:
	bin/file-system_manager/file-system_manager

assembler: scripts/src/assembler.cpp
	g++ scripts/src/assembler.cpp -o bin/assembler/assembler

compiler: scripts/src/compiler.cpp
	g++ scripts/src/compiler.cpp scripts/src/assembler.cpp -o bin/compiler/compiler

clean:
	rm -rf bin
