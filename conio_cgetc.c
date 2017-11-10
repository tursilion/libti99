#include "conio.h"
#include "kscan.h"

int conio_cursorFlag = 1;
int conio_cursorChar = 30;  // you can override this!
int conio_allowInts = 1;    // you can override this to disable Interrupts
extern unsigned char last_conio_key;    // in kbhit

#define BLINK_RATE 200

// no keyboard buffer on the TI, but to support kbhit() we will implement a single-key one
// requires a NEW keypress (or buffered from kbhit)
// Note this will turn interrupts OFF (if they were on) unless conio_allowInts is 0
// If you have interrupts ON, then you MUST disable the blinking cursor.
unsigned char cgetc() {
    unsigned char k = -1;
    int blink = -BLINK_RATE;

    if (last_conio_key != 255) {
        k = last_conio_key;
        last_conio_key = 255;
        return k;
    }

    do {
        if (conio_allowInts) {
            VDP_INT_POLL;
        }
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
        k = kscan(5);
    } while ((k == 255) || ((KSCAN_STATUS&KSCAN_MASK) == 0));

    vdpchar(conio_getvram(), ' ');
    return k;
}
