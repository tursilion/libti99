#include "vdp.h"

void sprite(int n, int ch, int col, int r, int c) {
	unsigned int adr=gSprite+(n<<2);
	VDP_SET_ADDRESS_WRITE(adr);
	VDPWD=r;
	VDPWD=c;
	VDPWD=ch;
	VDPWD=col;
}
