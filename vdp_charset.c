// VDP code for the TI-99/4A by Tursi
// You can copy this file and use it at will if it's useful

#include "vdp.h"

void charset() {
	gplvdp(0x004a, gPattern+0x300, 31);	// lower case letters
	gplvdp(0x0018, gPattern+0x100, 64);	// the rest of the character set (not shifted)
	vdpmemset(gPattern+(30*8), 0xfc, 8);	// cursor
}


