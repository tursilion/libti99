#include "conio.h"

void clrscr() {
    gotoxy(0,0); 
    vdpmemset(conio_getvram(), ' ', nTextEnd+1);
#ifdef ENABLE_F18A
    if (gColor == 0x800) {
	extern unsigned int conio_scrnCol; // conio_bgcolor.c
	vdpmemset(gColor, conio_scrnCol, nTextEnd+1);
    }
#endif
}
