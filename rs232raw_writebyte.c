// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// writes a byte to the specified serial port - does not check the transmission register!
// this does manipulate RTS/DCD since everyone does ;)
void rs232raw_writebyte(int rawCRU, int ch) {
    __asm__ (
        "MOV %0,R12\n\tSBO 16\n\tSWPB %1\n\tLDCR %1,8\n\tSBZ 16" : : "r" (rawCRU), "r" (ch) : "r12"
    );
}
