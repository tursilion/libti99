#include "vdp.h"

void writestring(int row, int col, char *pStr) {
	VDP_SET_ADDRESS_WRITE(gImage+(row<<5)+col);
	while (*pStr) {
		VDPWD = *(pStr++);
	}
}
