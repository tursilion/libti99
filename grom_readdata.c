#include "grom.h"

// multiple port GROM access code - this is slow for bulk access
// but will do for this test program, I don't expect that multiple
// reads/writes will be a problem if the cart works at all. ;)
unsigned char GromReadData(unsigned int address, unsigned char port) {
	// we only support 15 ports, this still fits in a char
	port <<= 2;

	// set address
	*((volatile unsigned char*)(GROMWA_0+port)) = address>>8;
	*((volatile unsigned char*)(GROMWA_0+port)) = address&0xff;

	// read data
	return *((volatile unsigned char*)(GROMRD_0+port));
}
