#include "vdp.h"

unsigned char vdpreadchar(int pAddr) {
	VDP_SET_ADDRESS(pAddr);
	__asm("NOP");
	return VDPRD;
}
