BIN = ..\bin
BIN_DRIVES = ${BIN}\drives
BIN_SOFTWARE = ${BIN}\software
BIN_TOOLS = ${BIN}\tools

CC = ${BIN_TOOLS}\compiler\release_windows_x86_64\compiler.exe
ASM = ${BIN_TOOLS}\assembler-app\release_windows_x86_64\assembler-app.exe

## C
bootloader: bootloader\bootloader.c
	${CC} bootloader\bootloader.c -a bootloader\bootloader.asm -o ${BIN_SOFTWARE}\bootloader.bin

testc: tests\test.c
	${CC} tests\test.c -a tests\test.asm

gpu_test: tests\gpu_test.c
	${CC} tests\gpu_test.c -e ofst_data 1 -a tests\gpu_test.asm -o ${BIN_SOFTWARE}\gpu_test.bin

kernel: kernel\kernel.c
	${CC} kernel\kernel.c -e size_seg -a kernel\kernel.asm -o ${BIN_SOFTWARE}\kernel.bin

## ASM
bios: bootloader\bios.asm
	${ASM} bootloader\bios.asm -o ${BIN_SOFTWARE}\bios.bin

testa: tests\tester.asm
	${ASM} tests\tester.asm -o ${BIN_SOFTWARE}\tester.bin
