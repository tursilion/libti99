// VDP code for the TI-99/4A by Tursi
// This is the generic code that all include.
// You can copy this file and use it at will if it's useful

// global pointers for all to enjoy - make sure the screen setup code updates them!
// assumptions here are for E/A environment, they may not be accurate and your
// program should NOT trust them
unsigned int gImage = 0x0000;		// SIT, Register 2 * 0x400
unsigned int gColor = 0x0380;		// CR,  Register 3 * 0x40
unsigned int gPattern = 0x0800;		// PDT, Register 4 * 0x800
unsigned int gSprite = 0x0300;		// SAL, Register 5 * 0x80
unsigned int gSpritePat = 0x0000;	// SDT, Register 6 * 0x800

