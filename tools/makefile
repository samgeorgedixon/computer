BIN = ../bin/tools

asm_rs: asm_rs\src\main.rs
	cargo build -p asm_rs --target-dir ${BIN}/asm_rs
	$(BIN)/asm_rs/debug/asm_rs.exe
