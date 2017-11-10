#include "conio.h"
#include "kscan.h"

int conio_cursorFlag = 1;
int conio_cursorChar = 30;  // you can override this!
extern unsigned char last_conio_key;    // in kbhit

#define BLINK_RATE 200

// no keyboard buffer on the TI, but to support kbhit() we will implement a single-key one
unsigned char cgetc() {
    unsigned char k = -1;
    int blink = -BLINK_RATE;

    if (last_conio_key != 255) {
        k = last_conio_key;
        last_conio_key = 255;
        return k;
    }

    do {
        k = kscan();
        if (conio_cursorFlag) {
            if (blink == -BLINK_RATE) {
                vdpchar(conio_getvram(), conio_cursorChar);
            }
            if (blink == 0) {
                vdpchar(conio_getvram(), ' ');
            }
            if (blink < BLINK_RATE) {
                ++blink;
            } else {
                blink = -BLINK_RATE;
            }
        }
    } while (k == 0xff);
    vdpchar(conio_getvram(), ' ');
    return k;
}
