#include "vdp.h"

unsigned char gchar(int r, int c) {
	VDP_SET_ADDRESS(gImage+(r<<5)+c);
	__asm("NOP");	// address write to read turnaround can be too short!
	return VDPRD;
}
