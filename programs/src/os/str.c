#include "../myc.h"

void CheckStrEqual(char strA[20], char strB[20], boolR equal) {
    int16_t i = 0;
    equal = true;

    while (i != -1) {
        if (strA[i] != strB[i]) {
            equal = false;
            i = -1;
        }
        if (strA[i] == 0) {
            i = -1;
        }
    }
}
