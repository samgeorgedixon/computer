#include "stdint.h"
#include "stdbool.h"

#include "src/os/drive.c"

char[7] LoadKernel() {
    char fileName[7];

    int16_t fileTableIndex = GetFileTableIndex(fileName);

    int16_t fileAddr = GetFileAddress(fileTableIndex);
    int16_t fileSize = GetFileSize(fileTableIndex);

    return fileName;
}

void main() {
    asm("li ds 2");
    asm("li ss 2");
    
    asm("li bp 0xff");
    asm("mov sp bp");

    char fileName[7];
    fileName = LoadKernel();

    while (true) {}
}
