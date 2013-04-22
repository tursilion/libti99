#include "vdp.h"

void vdpmemset(int pAddr, int ch, int cnt) {
	VDP_SET_ADDRESS_WRITE(pAddr);
	while (cnt--) {
		VDPWD = ch;
	}
}
