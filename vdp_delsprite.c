#include "vdp.h"

// just pushes it off screen, but not disabled
void delsprite(int n) {
	unsigned int adr=gSprite+(n<<2);
	VDP_SET_ADDRESS_WRITE(adr);
	VDPWD=192;
}
