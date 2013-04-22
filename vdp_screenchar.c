#include "vdp.h"

void vdpscreenchar(int pAddr, int ch) {
	VDP_SET_ADDRESS_WRITE(pAddr+gImage);
	VDPWD=ch;
}
