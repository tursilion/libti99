#include "vdp.h"

void vdpwritescreeninc(int pAddr, int nStart, int cnt) {
	VDP_SET_ADDRESS_WRITE(pAddr+gImage);
	while (cnt--) {
		VDPWD=nStart++;
	}
}
