#include "vdp.h"

void faster_hexprint(unsigned char x) {
	unsigned int dat = byte2hex[x];

	VDPWD = dat>>8;
	// we rely on the TI not needing a delay between the writes here...
	// this partially depends on the optimzer, but the fastest possible
	// code might break the back-to-back rules... still, it's a write.
	VDPWD = dat&0xff;
}
