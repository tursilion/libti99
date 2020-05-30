#include "vdp.h"

extern unsigned char vdp_bigbuf[256];

void fast_scrn_scroll() {
    // similar to the slow scrn_scroll, but uses a larger fixed
    // buffer for far more speed
    const int line = nTextEnd - nTextRow + 1;
    const int end = nTextEnd + gImage;
    int adr = gImage+line;
    int size = sizeof(vdp_bigbuf);

    while (adr+size < end) {
        vdpmemread(adr, vdp_bigbuf, size);
        vdpmemcpy(adr - line, vdp_bigbuf, size);
        adr += size;
    }
    if (adr <= end) {
        // copy whatever was left
        size = end - adr + 1;
        vdpmemread(adr, vdp_bigbuf, size);
        vdpmemcpy(adr - line, vdp_bigbuf, size);
    }
    vdpmemset(nTextRow, ' ', line);	// clear the last line
}
