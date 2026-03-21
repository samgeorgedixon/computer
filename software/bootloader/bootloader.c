#include "../myc.h"

asm("li ds 2");
asm("li ss 2");

asm("li bp 0xffff");
asm("mov sp bp");

#include "src/os/drive.c"

void PrintPixel(int16_t addr, int16_t colourRGB555) {
    asm("li es 0");
    asm("ldw s r1 [bp 5]");
    asm("ldw s r2 [bp 7]");
    
    asm("ste e2 r1 [r2 0]");
}

void FillScreen(int16_t colourRGB555) {
    int16_t addr = 0;
    bool done;

    while (done == 0) {
        PrintPixel(addr, colourRGB555);

        addr += 1;

        if (addr == 0) {
            done = 1;
        }
    }
}

void LoadKernel(charR name[6]) {
    char fileName[6];

    int16_t fileTableIndex;
    GetFileTableIndex(fileName, fileTableIndex);

    int16_t fileAddr;
    int16_t fileSize;
    GetFile(fileTableIndex, fileAddr, fileSize);

    FillScreen(fileAddr);

    name = fileName;
}

void main() {
    FillScreen(0b0111110000000000);

    char fileName[7];
    LoadKernel(fileName);

    while (true) {}
}
