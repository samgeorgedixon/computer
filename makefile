all: clean computer computer_run

computer: src/computer/computer.sv
	verilator -j 0 -O3 --x-assign fast --x-initial fast --noassert -I../ \
		--Mdir bin --trace --timing -cc --exe \
		src/computer/computer.sv src/main.cpp src/testbench/application.cpp src/load_rom_file.cpp -o computer \
		-CFLAGS "-I/usr/include/SDL2 -D_REENTRANT" -LDFLAGS "-lSDL2"

	make -C ./bin -f Vcomputer.mk

computer_run:
	bin/computer

clean:
	rm -rf bin
