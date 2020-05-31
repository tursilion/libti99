#include "vdp.h"

void writestring(int row, int col, char *pStr) {
	if (nTextFlags&TEXT_WIDTH_32) {
		VDP_SET_ADDRESS_WRITE(VDP_SCREEN_POS(row,col)+gImage);
	} else if (nTextFlags&TEXT_WIDTH_40) {
		VDP_SET_ADDRESS_WRITE(VDP_SCREEN_TEXT(row,col)+gImage);
	} else if (nTextFlags&TEXT_WIDTH_80) {
		VDP_SET_ADDRESS_WRITE(VDP_SCREEN_TEXT80(row,col)+gImage);
	} else if (nTextFlags&TEXT_WIDTH_64) {
		// TODO: this doesn't make any sense, does it??
		VDP_SET_ADDRESS_WRITE(VDP_SCREEN_TEXT64(row,col)+gImage);
	} else {
		return;
	}
	while (*pStr) {
		VDPWD = *(pStr++);
	}
}
