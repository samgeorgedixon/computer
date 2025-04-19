#include "stdint.h"
#include "stdbool.h"

#include "scripts/programs/os/str.c"

int8_t GetDriveByte(int16_t addr, int16_t segment) {
    asm("ldw s r2 [bp 9]");
    asm("ldw s es [bp 7]");
    
    asm("lde e1 r1 [r2 0]");

    asm("stb s r1 [bp 6]");
}
int16_t GetDriveWord(int16_t addr, int16_t segment) {
    asm("ldw s r2 [bp 10]");
    asm("ldw s es [bp 8]");
    
    asm("lde e1 r1 [r2 0]");

    asm("stw s r1 [bp 6]");
}

int16_t GetFileTableIndex(char name[7]) {
    int16_t i = 0;
    while (i != 250) {
        char fileName[7];

        int16_t j = 0;
        while (j != 6) {
            int16_t k = i;
            k += j;
            fileName[j] = GetDriveByte(k, 0);

            j += 1;
        }
        bool equal = CheckStrEqual(name, fileName);

        if (equal) {
            return i;
        }

        i += 10;
    }
}

int16_t GetFileAddress(int16_t tableIndex) {
    tableIndex += 6;
    int16_t addr = GetDriveWord(tableIndex, 0);
    return addr;
}

int16_t GetFileSize(int16_t tableIndex) {
    tableIndex += 8;
    int16_t size = GetDriveWord(tableIndex, 0);
    return size;
}
