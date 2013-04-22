#include "vdp.h"

void vdpmemcpy(int pAddr, const unsigned char *pSrc, int cnt) {
	VDP_SET_ADDRESS_WRITE(pAddr);
	while (cnt--) {
		VDPWD=*(pSrc++);
	}
}
