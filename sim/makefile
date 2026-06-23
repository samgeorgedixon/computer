BIN = ../bin
BIN_COMPUTER = ${BIN}/computer

## Dependencies: wsl2, build-essential, verilator, libsdl2-dev

all: build run

build: src/computer.sv
	verilator -j 0 -O3 --x-assign fast --x-initial fast --noassert -I../ \
		--Mdir computer --trace-fst --timing -cc --exe \
		src/computer.sv main.cpp testbench/application.cpp testbench/computer.cpp testbench/load-memory-file.cpp -o computer \
		-CFLAGS "-I/usr/include/SDL2 -D_REENTRANT" -LDFLAGS "-lSDL2"

	make -C ./computer -f Vcomputer.mk

	rm -rf ${BIN_COMPUTER}
	mv computer ${BIN}

run:
	${BIN_COMPUTER}/computer
