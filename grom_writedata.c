#include "grom.h"

void GromWriteData(unsigned int address, unsigned char port, unsigned char dat) {
	// we only support 15 ports, this still fits in a char
	port <<= 2;

	// set address
	*((volatile unsigned char*)(GROMWA_0+port)) = address>>8;
	*((volatile unsigned char*)(GROMWA_0+port)) = address&0xff;

	// write data
	*((volatile unsigned char*)(GROMWD_0+port)) = dat;
}
