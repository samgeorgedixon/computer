#include "inttypes.h"
#include "stdbool.h"

void find_file(int16_t startSeg, int16_t sizeSeg, char name[6]) {
    for (int i = 0; i < 260; i += 10) {
        bool isThis = false;

        for (int j = 0; j < 6; j++) {
            char charr;

            if (charr == name[j]) {
                isThis = true;
            } else {
                break;
            }
        }
        if (isThis) {
            // JMP
        }
    }
}
