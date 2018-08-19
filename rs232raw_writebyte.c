// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// writes a byte to the specified serial port - does not check the transmission register!
// this does manipulate RTS/DCD since everyone does ;)
void rs232raw_writebyte(int rawCRU, int ch) {
    __asm__ (
        "  mov %0,r12\n"	// get rawCRU
        "  sbo 16\n"		// set RTS low (this is what the card and FlipSide did...? maybe enables the write?)
        "  swpb %1\n"		// get the value into the MSB
        "  ldcr %1,8\n"		// write the byte
        "  sbz 16\n"		// set RTS high (will happen after transmission)
        "  swpb %1\n"		// fix the reg in case gcc wants it
        : : "r" (rawCRU), "r" (ch) : "r12"
    );
}
