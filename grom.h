// GROM code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#ifndef GROM_H
#define GROM_H

//*********************
// default GROM access port addresses (base 0)
//*********************

// Read Data
#define GROMRD_0			(0x9800)
// Read Address
#define GROMRA_0			(0x9802)
// Write Data
#define GROMWD_0			(0x9C00)
// Write Address
#define GROMWA_0			(0x9c02)

// Note: additional ports are spaced 4 bytes apart

//*********************
// Default (base 0) GROM access ports
//*********************

// Read Data
#define GROMRD			*((volatile unsigned char*)GROMRD_0)
// Read Address
#define GROMRA			*((volatile unsigned char*)GROMRA_0)
// Write Data
#define GROMWD			*((volatile unsigned char*)GROMWD_0)
// Write Address
#define GROMWA			*((volatile unsigned char*)GROMWA_0)

//*********************
// Default GROM access macros
//*********************

// Set the GROM address - this value will be used for read or write
inline void GROM_SET_ADDRESS(unsigned int x)		{	GROMWA=((x)>>8); GROMWA=((x)&0xff);							}

// Get the current GROM address - this value will be one higher than expected due to internal prefetch.
// Reading the address destroys it, so you must set the address again afterwards.
inline unsigned int GROM_GET_ADDRESS()				{	unsigned int ret; ret=(GROMRA)<<8; ret|=GROMRA; return ret; }

//*********************
// More comprehensive GROM functions (multiple ports)
//*********************

// Read a byte from GROM at the specified address and port (use port 0 if you want the default).
// Inputs: GROM address to read (0x0000-0xFFFF), Port number (0-15)
// Note that console GROMs and most cartridges will respond at all ports. The maximum port supported
// by this library is 15.
unsigned char GromReadData(unsigned int address, unsigned char port);

// Write a byte to GROM at the specified address and port (use port 0 if you want the default).
// Inputs: GROM address to read (0x0000-0xFFFF), Port number (0-15), value to write
// Note that console GROMs and most cartridges will respond at all ports. The maximum port supported
// by this library is 15. Assumes that writable GROM is available.
void GromWriteData(unsigned int address, unsigned char port, unsigned char dat);

#endif /* GROM_H */
