#include "vdp.h"

void fast_hexprint(unsigned char x) {
	char buf[3];
	unsigned int dat = byte2hex[x];

	buf[0] = dat>>8;
	buf[1] = dat&0xff;
	buf[2]='\0';

	putstring(buf);
}
