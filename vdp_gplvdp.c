#include "vdp.h"
#include "grom.h"

// Copy cnt characters from a GPL copy function vectored at
// vect to VDP adr. GPL vector must be a B or BR and
// the first actual instruction must be a DEST with an
// immediate operand. 994A only (99/4 uses 6 byte chars)
void gplvdp(int vect, int adr, int cnt) {
	GROM_SET_ADDRESS(vect);
	vect=(GROMRD<<8);		// get pointed to vector address
	vect|=GROMRD;			// must be two lines, because otherwise order is not guaranteed!

	vect &= 0x1fff;			// mask out instruction
	vect+=3;				// skip instruction and destination bytes

	GROM_SET_ADDRESS(vect);	// set new GROM address
	vect=(GROMRD<<8);		// get actual address of the table
	vect|=GROMRD;			// must be two lines, because otherwise order is not guaranteed!
	GROM_SET_ADDRESS(vect);	// and set that GROM address - GROM is now ready!

	VDP_SET_ADDRESS_WRITE(adr);	// VDP is now ready!

	while (cnt--) {
		VDPWD = 0;			// pad the top of the char with a space
		for (int idx=0; idx<7; idx++) {		// then copy 7 bytes
			VDPWD = GROMRD;	// copy a byte (both sides autoincrement)
		}
	}
}
