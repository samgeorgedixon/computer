BIN = ..\bin
BIN_DRIVES = ${BIN}\drives
BIN_SOFTWARE = ${BIN}\software
BIN_TOOLS = ${BIN}\tools

CC = ${BIN_TOOLS}\compiler\release-windows\compiler.exe
ASM = ${BIN_TOOLS}\assembler-app\release-windows\assembler-app.exe

## C
bootloader: bootloader\bootloader.c
	${CC} bootloader\bootloader.c -a bootloader\bootloader.asm -o ${BIN_SOFTWARE}\bootloader.bin

testc: tests\test.c
	${CC} tests\test.c -a tests\test.asm

kernel: kernel\kernel.c
	${CC} kernel\kernel.c -e size_seg -a kernel\kernel.asm -o ${BIN_SOFTWARE}\kernel.bin

## ASM
bios: bootloader\bios.asm
	${ASM} bootloader\bios.asm -o ${BIN_SOFTWARE}\bios.bin

new: tests\new.asm
	${ASM} tests\new.asm -o ${BIN_SOFTWARE}\new.bin

gpu_test: tests\gpu_test.asm
	${ASM} tests\gpu_test.asm -o ${BIN_SOFTWARE}\gpu_test.bin

testa: tests\tester.asm
	${ASM} tests\tester.asm -o ${BIN_SOFTWARE}\tester.bin

