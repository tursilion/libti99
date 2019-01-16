// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// reads a byte from the specified serial port - whatever is there. Does not check or
// block, so if you need correct data do a poll first!
int rs232raw_readbyte(int rawCRU) {
    int ret;

    __asm__ (
        "  mov %1,r12\n"	// get rawCRU
        "  clr %0\n"		// clear rx register
        "  stcr %0,8\n"		// get byte
        "  swpb %0\n" 		// make LSB
        "  sbz 18\n"		// reset rx flag
        : "=rm" (ret) : "r" (rawCRU) : "r12" 
    );

    return ret;
}
