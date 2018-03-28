#include "vdp.h"

static unsigned char bigbuf[256];

void fast_scrn_scroll() {
	// similar to the slow_scrn_scroll, but uses a larger fixed
    // buffer for far more speed
    const int line = nTextEnd - nTextRow + 1;
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

#ifdef ENABLE_F18A
    if (gColor == 0x800) {
	// copy color attributes the same way
	extern unsigned int conio_scrnCol; // conio_bgcolor.c
	const int end = nTextEnd + gColor;
	int adr = gColor + line;
	
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
	vdpmemset(nTextRow + gColor, conio_scrnCol, line);	// clear the last line
    }
#endif
}
