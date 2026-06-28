#include "tests/blah.c"

int16_t h = 2;
int8_t hdj[4];

void hello(int16_t i, int8_t j) {
    return;
}

void main() {
    asm("li r4 5");

    int16_t arra[10];
    int16_t arrc[10];
    int8_t arrb[5];

    arra[2] = 11;
    arrb[4] = 1;

    hdj[0] = 3;

    int16_t a = 5;
    int16_t b = 2;

    a += 1;

    a -= b;
    a *= h;

    if (a == 6) {
        Blah(a);
    }

    hello(a, h, arrb);

    while (true) {}
}
