CC = ..\bin\compiler\release_windows_x86_64\compiler.exe
ASM = ..\bin\assembler-app\release_windows_x86_64\assembler-app.exe

BUILD_DIR = bin\programs

# CC

bootloader: src\os\bootloader.c
	${CC} src\os\bootloader.c -a src\os\bootloader.asm -o ${BUILD_DIR}\bootloader.bin

testc: src\tests\test.c
	${CC} src\tests\test.c -a src\tests\test.asm

# ASM

testa: src\tests\tester.asm
	${ASM} src\tests\tester.asm -o ${BUILD_DIR}\tester.bin
