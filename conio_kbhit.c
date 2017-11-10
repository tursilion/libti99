#include "conio.h"

// single key buffer that cgetc can check
unsigned char last_conio_key = 255;

unsigned char kbhit() {
    unsigned char k = kscan();

    if (k != 255) {
        last_conio_key = k;
        return 1;
    } else {
        return 0;
    }
}
