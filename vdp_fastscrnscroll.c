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
        while (VDPST & 0x80);    // wait for GPU status to be idle
        VDP_SET_REGISTER(0x0f,0); // status register to read = SR0

        extern unsigned int conio_scrnCol; // conio_bgcolor.c
        vdpmemset(nTextRow + gColor, conio_scrnCol, line);  // clear the last line

        return;
    }
#endif

#ifdef ENABLE_TEXT64
    if (nTextRow == 64*23) {
        extern unsigned int conio_scrnCol; // conio_bgcolor.c
        extern int text64_scroll;
        unsigned char *buf = (unsigned char *)0x8320;
        int i, src, dst, ch;

        src = (8*32 + text64_scroll) * 8; // source = first middle
        dst = (0*32 + text64_scroll) * 8; // dest = last top

        text64_scroll = (text64_scroll + 0x20) & 0x00e0;
        ch = text64_scroll;

        VDP_SET_ADDRESS_WRITE(gImage);
        for (i = 0; i < 7*32; i++) {
            VDPWD = ch++;
        }

        // copy 32 patterns from middle to top
        vdpmemread(gPattern + src, bigbuf, 256);
        vdpmemcpy(gPattern + dst, bigbuf, 256);
        vdpmemread(gColor + src, bigbuf, 256);
        vdpmemcpy(gColor + dst, bigbuf, 256);

        VDP_SET_ADDRESS_WRITE(gImage + 7*32);
        for (i = 0; i < 8*32; i++) {
            VDPWD = ch++;
        }

        // copy 32 patterns from bottom to middle
        src += 8*32*8;
        dst += 8*32*8;
        vdpmemread(gPattern + src, bigbuf, 256);
        vdpmemcpy(gPattern + dst, bigbuf, 256);
        vdpmemread(gColor + src, bigbuf, 256);
        vdpmemcpy(gColor + dst, bigbuf, 256);

        VDP_SET_ADDRESS_WRITE(gImage + 15*32);
        for (i = 0; i < 9*32; i++) {
            VDPWD = ch++;
        }

        // clear the last line
        dst += 8*32*8;
        vdpmemset(gPattern + dst, 0, 256);
        vdpmemset(gColor + dst, conio_scrnCol, 256);


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
