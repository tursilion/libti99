#include "vdp.h"

static unsigned char bigbuf[256];

void fast_scrn_scroll() {
    // similar to the slow_scrn_scroll, but uses a larger fixed
    // buffer for far more speed
    const int line = nTextEnd - nTextRow + 1;

#ifdef ENABLE_F18A
    if (gColor == 0x800) {
        // use GPU code for fastest scrolling
        VDP_SET_REGISTER(0x38,1); // trigger GPU code

        VDP_SET_REGISTER(0x0f,2); // status register to read = SR2
        while (VDPST & 0x80) {    // wait for GPU status to be idle
        }
        VDP_SET_REGISTER(0x0f,0); // status register to read = SR0

        extern unsigned int conio_scrnCol; // conio_bgcolor.c
        vdpmemset(nTextRow + gColor, conio_scrnCol, line);  // clear the last line

        return;
    }
#endif

    const int end = nTextEnd + gImage;
    int adr = gImage+line;
    int size = sizeof(bigbuf);

    while (adr+size < end) {
        vdpmemread(adr, bigbuf, size);
        vdpmemcpy(adr - line, bigbuf, size);
        adr += size;
    }
    if (adr <= end) {
        // copy whatever was left
        size = end - adr + 1;
        vdpmemread(adr, bigbuf, size);
        vdpmemcpy(adr - line, bigbuf, size);
    }
    vdpmemset(nTextRow, ' ', line);	// clear the last line

}
