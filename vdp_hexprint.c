#include "vdp.h"

void hexprint(unsigned char x) {
	char buf[3];

	buf[0]=(x>>4)+'0';
	if (buf[0]>'9') buf[0]+=7;
	buf[1]=(x&0x0f)+'0';
	if (buf[1]>'9') buf[1]+=7;
	buf[2]='\0';
	putstring(buf);
}
