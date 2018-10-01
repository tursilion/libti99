#include "conio.h"

// TODO: optimize through the header/client compile time.

void clrscr() {
    // only needed for 64 column bitmap text mode
    if (nTextRow == 64*23) {
	vdpmemset(gPattern, 0, 32*24*8);
        extern unsigned int conio_scrnCol; // conio_bgcolor.c
	vdpmemset(gColor, conio_scrnCol, 32*24*8);
        return;
    }

    // used by all text/tile modes
    gotoxy(0,0); 
    vdpmemset(conio_getvram(), ' ', nTextEnd+1);

    // only needed for 80 column color mode
    if (gColor == 0x800) {
	extern unsigned int conio_scrnCol; // conio_bgcolor.c
	vdpmemset(gColor, conio_scrnCol, nTextEnd+1);
    }
}
