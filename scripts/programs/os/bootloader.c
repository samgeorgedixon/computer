#include "stdint.h"
#include "stdbool.h"

asm("li ds 2");
asm("li ss 2");

asm("li bp 0xff");
asm("mov sp bp");

#include "scripts/programs/os/drive.c"

void LoadKernel() {
    char fileName[7];

    int16_t fileTableIndex = GetFileTableIndex(fileName);

    int16_t fileAddr = GetFileAddress(fileTableIndex);
    int16_t fileSize = GetFileSize(fileTableIndex);
}

void main() {
    LoadKernel();

    while (true) {}
}
