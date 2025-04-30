#include "../myc.h"

#include "src/os/str.c"

void GetDriveByte(int16_t addr, int16_t segment[3], int8_tR byte) {
    asm("ldw s r2 [bp 9]");
    asm("ldw s es [bp 7]");
    
    asm("lde e1 r1 [r2 0]");

    asm("stb s r1 [bp 6]");
}
void GetDriveWord(int16_t addr, int16_t segment, int16_tR word) {
    asm("ldw s r2 [bp 10]");
    asm("ldw s es [bp 8]");
    
    asm("lde e1 r1 [r2 0]");

    asm("stw s r1 [bp 6]");
}

void GetFileTableIndex(char name[7], int16_tR index) {
    int16_t i = 0;
    while (i != 250) {
        char fileName[7];

        int16_t j = 0;
        while (j != 6) {
            int16_t k = i;
            k += j;

            GetDriveByte(k, 0, fileName[j]);

            j += 1;
        }
        bool equal;
        CheckStrEqual(name, fileName, equal);

        if (equal) {
            index = i;
            return;
        }

        i += 10;
    }
}

void GetFile(int16_t tableIndex, int16_tR addr, int16_tR size) {
    tableIndex += 6;
    GetDriveWord(tableIndex, 0, addr);

    tableIndex += 2;
    GetDriveWord(tableIndex, 0, size);
}
