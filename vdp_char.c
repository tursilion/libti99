#include "vdp.h"


#if defined(ENABLE_F18A) || defined(ENABLE_TEXT64)

void vdpchar_default(int pAddr, int ch) {
	VDP_SET_ADDRESS_WRITE(pAddr);
	VDPWD=ch;
}

void (*vdpchar)(int pAddr, int ch) = vdpchar_default;

#else

void vdpchar(int pAddr, int ch) {
	VDP_SET_ADDRESS_WRITE(pAddr);
	VDPWD=ch;
}

#endif
