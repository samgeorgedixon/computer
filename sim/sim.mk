all: computer-build computer-run

computer-build: src/computer/computer.sv
	verilator -j 0 -O3 --x-assign fast --x-initial fast --noassert -I../ \
		--Mdir computer --trace --timing -cc --exe \
		src/computer/computer.sv src/main.cpp src/testbench/application.cpp src/testbench/computer.cpp src/load_rom_file.cpp -o computer \
		-CFLAGS "-I/usr/include/SDL2 -D_REENTRANT" -LDFLAGS "-lSDL2"

	make -C ./computer -f Vcomputer.mk

	rm -rf ./bin/computer
	mv computer ./bin
computer-run:
	../bin/computer/computer
