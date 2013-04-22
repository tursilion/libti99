#include "vdp.h"

void vchar(int r, int c, int ch, int cnt) {
	int pAddr = gImage+(r<<5)+c;
	while (cnt--) {
		vdpchar(pAddr, ch);
		pAddr+=32;
	}
}
