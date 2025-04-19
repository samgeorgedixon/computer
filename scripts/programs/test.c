#include "stdbool.h"
#include "stdint.h"

#include "scripts/programs/blah.c"

int16_t h = 2;

void main() {
    asm("li r4 5");

    int16_t a = 5;
    int16_t b = 2;

    a += 1;

    a -= b;
    a *= h;

    if (a == 6) {
        a = Blah(a);
    }

    while (true) {}
}
