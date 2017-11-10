#include "conio.h"
#include "kscan.h"

// single key buffer that cgetc can check
unsigned char last_conio_key = 255;

// returns true and fills in the buffer if a NEW key is hit,
// or if the buffer still holds the last key
unsigned char kbhit() {
    if (last_conio_key != 255) {
        return 1;
    }

    unsigned char k = kscan(5);
    if ((k != 255)&&(KSCAN_STATUS&KSCAN_MASK)) {
        // new keypress active
        last_conio_key = k;
        return 1;
    } else {
        return 0;
    }
}
