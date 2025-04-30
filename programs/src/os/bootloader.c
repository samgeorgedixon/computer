#include "../myc.h"

#include "src/os/drive.c"

void LoadKernel(charR name[7]) {
    char fileName[7];

    int16_t fileTableIndex;
    GetFileTableIndex(fileName, fileTableIndex);

    int16_t fileAddr;
    int16_t fileSize;
    GetFile(fileTableIndex, fileAddr, fileSize);

    return fileName;
}

void main() {
    asm("li ds 2");
    asm("li ss 2");
    
    asm("li bp 0xff");
    asm("mov sp bp");

    char fileName[7];
    LoadKernel(fileName);

    while (true) {}
}
