#include "../myc.h"

asm("li ds 2");
asm("li ss 2");

asm("li bp 0xffff");
asm("mov sp bp");

#include "src/os/drive.c"

void LoadKernel(char name[6]) {
    char fileName[6];

    int16_t fileTableIndex;
    GetFileTableIndex(fileName, fileTableIndex);

    int16_t fileAddr;
    int16_t fileSize;
    GetFile(fileTableIndex, fileAddr, fileSize);

    return fileName;
}

void main() {
    char fileName[7];
    LoadKernel(fileName);

    while (true) {}
}
