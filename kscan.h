// KSCAN definitions for keyboard and joystick
// These have to do with the console's keyboard read function, which
// requires both ROM and GROM access.

//*********************
// KSCAN related addresses
//*********************

// Address to set the scan mode (see KSCAN_MODE_xxx defines)
#define KSCAN_MODE	*((volatile unsigned char*)0x8374)

// Address to read back the detected key. 0xFF if no key was pressed.
#define KSCAN_KEY	*((volatile unsigned char*)0x8375)

// Address to read back the joystick X axis (scan modes 1 and 2 only)
#define KSCAN_JOYX	*((volatile unsigned char*)0x8376)

// Address to read back the joystick Y axis (scan modes 1 and 2 only)
#define KSCAN_JOYY	*((volatile unsigned char*)0x8377)

// Address to check the status byte. KSCAN_MASK is set if a key was pressed
#define KSCAN_STATUS *((volatile unsigned char*)0x837c)
#define KSCAN_MASK	0x20

//*********************
// KSCAN modes
//*********************

#define KSCAN_MODE_LAST		0		// last mode scanned
#define KSCAN_MODE_LEFT		1		// left side of keyboard and joystick 1 (fire is a key of 18)
#define KSCAN_MODE_RIGHT	2		// right side of keyboard and joystick 2 (fire is a key of 18)
#define KSCAN_MODE_994		3		// upper-case only, 99/4 compatible results
#define KSCAN_MODE_PASCAL	4		// PASCAL mapping, different control keys
#define KSCAN_MODE_BASIC	5		// Normal 99/4A BASIC mode


//*********************
// Joystick return values
//*********************

#define JOY_LEFT	0xfc
#define JOY_RIGHT	0x04
#define JOY_UP		0x04
#define JOY_DOWN	0xfc


//*********************
// Function definitions
//*********************

unsigned char kscan(unsigned char mode);
