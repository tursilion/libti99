// Fast keyboard scan for the TI-99/4A by Tursi aka Mike Brent
// This code and library released into the Public Domain
// You can copy this file and use it at will ;)

#include "kscan.h"

// By columns, then rows. 8 Rows per column. No shift states
const unsigned char keymap[] = {
		61,32,13,255,1,2,3,255,
		'.','L','O','9','2','S','W','X',
		',','K','I','8','3','D','E','C',
		'M','J','U','7','4','F','R','V',
		'N','H','Y','6','5','G','T','B',
		'/',';','P','0','1','A','Q','Z'
};

void kscanfast(int mode) {
	if (mode == 0) {
		KSCAN_KEY = 0xff;
		for (unsigned int col=0; col < 0x0600; col += 0x0100) {
			unsigned int key;
			__asm__("li r12,>0024\n\tldcr %1,3\n\tsrc r12,7\n\tli r12,>0006\n\tclr %0\n\tstcr %0,8" : "=r"(key) : "r"(col) : "r12");	// set cru, column, delay, read
			unsigned int shift=0x8000;

			for (unsigned int cnt=0; cnt<8; cnt++) {
				// a pressed key returns a 0 bit
				if (key & shift) {
					shift>>=1;
					continue;
				}
				// found one
				KSCAN_KEY = keymap[(col>>5)+cnt];
				return;
			}
		}
	} else {
		unsigned int key;

		int col = 6;		// joystick 1 fire column

		if (mode == 2) {
			col = 7;		// make that joystick 2
		}

		__asm__("li r12,>0024\n\tldcr %1,3\n\tsrc r12,7\n\tli r12,>0006\n\tclr %0\n\tstcr %0,1" : "=r"(key) : "r"(col) : "r12");	// set cru, column, delay, read (only need 1 bit)
		if (key == 0) {
			KSCAN_KEY = 18;
		} else {
			KSCAN_KEY = 0xff;
		}
	}
}