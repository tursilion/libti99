#include "kscan.h"

unsigned char kscan(unsigned char mode) {
	KSCAN_MODE = mode;
	__asm__("LWPI >83E0\n\tBL @>000E\n\tLWPI >8300");
	return KSCAN_KEY;
}
