#include "vdp.h"

void writestring(int row, int col, char *pStr) {
	if (nTextFlags&TEXT_WIDTH_32) {
		VDP_SET_ADDRESS_WRITE(VDP_SCREEN_POS(row,col)+gImage);
	} else if (nTextFlags&TEXT_WIDTH_40) {
		VDP_SET_ADDRESS_WRITE(VDP_SCREEN_TEXT(row,col)+gImage);
	} else if (nTextFlags&TEXT_WIDTH_80) {
		VDP_SET_ADDRESS_WRITE(VDP_SCREEN_TEXT80(row,col)+gImage);
	} else if (nTextFlags&TEXT_WIDTH_64) {
        // we can use a slower version here... we really have no choice
        int adr = VDP_SCREEN_TEXT64(row,col);
	    while (*pStr) {
            vdpchar(adr++, *(pStr++));
	    }
        return;
	} else {
		return;
	}

    // write the string to the tiles
	while (*pStr) {
		VDPWD = *(pStr++);
	}
}
