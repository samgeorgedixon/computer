#include "stdint.h"
#include "stdbool.h"

bool CheckStrEqual(char strA[20], char strB[20]) {
    int16_t i = 0;
    bool equal = true;

    while (i != -1) {
        if (strA[i] != strB[i]) {
            equal = false;
            i = -1;
        }
        if (strA[i] == 0) {
            i = -1;
        }
    }
    return equal;
}
