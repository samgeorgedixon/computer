all: computer-build cpu_instr_gen-build assembler

computer: computer-build computer-run

computer-build: src/computer/computer.sv
	verilator -j 0 -O3 --x-assign fast --x-initial fast --noassert -I../ \
		--Mdir computer --trace --timing -cc --exe \
		src/computer/computer.sv src/main.cpp src/testbench/application.cpp src/load_rom_file.cpp -o computer \
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

assembler: scripts/src/assembler.cpp
	g++ scripts/src/assembler.cpp -o assembler

clean:
	rm -rf bin
