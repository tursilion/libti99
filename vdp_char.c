#include "vdp.h"


void vdpchar_default(int pAddr, int ch) {
	VDP_SET_ADDRESS_WRITE(pAddr);
	VDPWD=ch;
}

void (*vdpchar)(int pAddr, int ch) = vdpchar_default;

