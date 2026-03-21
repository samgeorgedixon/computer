BIN = ../bin
BIN_COMPUTER = ${BIN}/computer

all: computer-build computer-run

computer-build: src/computer.sv
	verilator -j 0 -O3 --x-assign fast --x-initial fast --noassert -I../ \
		--Mdir computer --trace --timing -cc --exe \
		src/computer.sv main.cpp testbench/application.cpp testbench/computer.cpp testbench/load_rom_file.cpp -o computer \
		-CFLAGS "-I/usr/include/SDL2 -D_REENTRANT" -LDFLAGS "-lSDL2"

	make -C ./computer -f Vcomputer.mk

	rm -rf ${BIN_COMPUTER}
	mv computer ${BIN}

computer-run:
	${BIN_COMPUTER}/computer
