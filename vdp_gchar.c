#include "vdp.h"

unsigned char gchar(int r, int c) {
	VDP_SET_ADDRESS(gImage+(r<<5)+c);
	return VDPRD;
}
