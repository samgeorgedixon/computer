CC = ..\bin\compiler\release_windows_x86_64\compiler.exe
ASM = ..\bin\assembler-app\release_windows_x86_64\assembler-app.exe

BUILD_DIR = bin\programs

# CC

bootloader: src\os\bootloader.c
	${CC} src\os\bootloader.c -a src\os\bootloader.asm -o ${BUILD_DIR}\bootloader.bin

testc: src\tests\test.c
	${CC} src\tests\test.c -a src\tests\test.asm

gpu_test: src\tests\gpu_test.c
	${CC} src\tests\gpu_test.c -e ofst_data 1 -a src\tests\gpu_test.asm -o ${BUILD_DIR}\gpu_test.bin

kernel: src\os\kernel.c
	${CC} src\os\kernel.c -e size_seg -a src\os\kernel.asm -o ${BUILD_DIR}\kernel.bin

# ASM

bios: src\bios.asm
	${ASM} src\bios.asm -o ${BUILD_DIR}\bios.bin

testa: src\tests\tester.asm
	${ASM} src\tests\tester.asm -o ${BUILD_DIR}\tester.bin
