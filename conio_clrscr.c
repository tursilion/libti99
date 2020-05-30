#include "conio.h"

// TODO: optimize through the header/client compile time.
extern unsigned int conio_scrnCol; // conio_bgcolor.c

void clrscr() {
    // special handling for the bitmap modes
    if (nTextFlags&TEXT_FLAG_IS_BITMAPPED) {
        // right now we assume no masking
	    vdpmemset(gPattern, 0, 32*24*8);
        if (nTextFlags&TEXT_FLAG_HAS_ATTRIBUTES) {
	        vdpmemset(gColor, conio_scrnCol, 32*24*8);
        }
    } else {
        // use the conio functions for text modes
        gotoxy(0,0);
        vdpmemset(conio_getvram(), ' ', nTextEnd+1);
        if (nTextFlags&TEXT_FLAG_HAS_ATTRIBUTES) {
    	    vdpmemset(gColor, conio_scrnCol, nTextEnd+1);
        }
    }
}
