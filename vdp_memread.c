#include "vdp.h"

void vdpmemread(int pAddr, unsigned char *pDest, int cnt) {
	VDP_SET_ADDRESS(pAddr);
	while (cnt--) {
		*(pDest++)=VDPRD;
	}
}
