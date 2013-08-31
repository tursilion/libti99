// Fast keyboard scan for the TI-99/4A by Tursi aka Mike Brent
// This code and library released into the Public Domain
// You can copy this file and use it at will ;)

#include "kscan.h"

void joystfast(int unit) {
	unsigned int result;

	// read the joystick lines (column 6 or 7, (5 added to unit))
	__asm__("li r12,>0024\n\tai %1,5\n\tldcr %1,3\n\tsrc r12,7\n\tli r12,>0006\n\tclr %0\n\tstcr %0,8" : "=r"(result) : "r"(unit) : "r12");

	KSCAN_JOYY = 0;
	KSCAN_JOYX = 0;

	if ((result & 0x0200) == 0) KSCAN_JOYX = JOY_LEFT;
	if ((result & 0x0400) == 0) KSCAN_JOYX = JOY_RIGHT;
	if ((result & 0x0800) == 0) KSCAN_JOYY = JOY_DOWN;
	if ((result & 0x1000) == 0) KSCAN_JOYY = JOY_UP;
}
