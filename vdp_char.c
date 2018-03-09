#include "vdp.h"

void vdpchar(int pAddr, int ch) {
	VDP_SET_ADDRESS_WRITE(pAddr);
	VDPWD=ch;
#ifdef ENABLE_F18A_ECM
    if (gColor == 0x800) {
		extern unsigned int conio_scrnCol; // conio_bgcolor.c
		VDP_SET_ADDRESS_WRITE(pAddr-gImage+gColor);
		VDPWD=conio_scrnCol;
	}
#endif
}
