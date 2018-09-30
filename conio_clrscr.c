#include "conio.h"

void clrscr() {
#ifdef ENABLE_TEXT64
    if (nTextRow == 64*23) {
	vdpmemset(gPattern, 0, 32*24*8);
        extern unsigned int conio_scrnCol; // conio_bgcolor.c
	vdpmemset(gColor, conio_scrnCol, 32*24*8);
        return;
    }
#endif
    gotoxy(0,0); 
    vdpmemset(conio_getvram(), ' ', nTextEnd+1);
#ifdef ENABLE_F18A
    if (gColor == 0x800) {
	extern unsigned int conio_scrnCol; // conio_bgcolor.c
	vdpmemset(gColor, conio_scrnCol, nTextEnd+1);
    }
#endif
}
