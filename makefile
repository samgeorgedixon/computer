all: computer computer_run

computer: src/computer_tb.sv
	verilator --binary --trace -j 0 src/computer_tb.sv src/test.cpp --top ComputerTB

computer_run:
	obj_dir/VComputerTB
