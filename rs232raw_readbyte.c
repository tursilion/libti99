// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// reads a byte from the specified serial port - whatever is there. Does not check or
// block, so if you need correct data do a poll first!
int rs232raw_readbyte(int rawCRU) {
    int ret;

    __asm__ (
        "MOV %1,R12\n\tCLR %0\n\tSTCR %0,8\n\tSWPB %0" : "=rm" (ret) : "r" (rawCRU) : "r12" 
    );

    return ret;
}
