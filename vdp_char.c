#include "vdp.h"

void vdpchar(int pAddr, int ch) {
	VDP_SET_ADDRESS_WRITE(pAddr);
	VDPWD=ch;
}
