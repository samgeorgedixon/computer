#include "../myc.h"

void PrintPixel(int16_t addr, int16_t colourRGB555) {
    asm("li es 0");
    asm("ldw s r1 [bp 5]");
    asm("ldw s r2 [bp 7]");
    
    asm("ste e2 r1 [r2 0]");
}

int16_t colour = 0b0111110000011111;

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

void main() {
    FillScreen(colour);
    
    while (true) {}
}

