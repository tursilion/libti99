// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// return the error bits from the specified port (we don't check the timer here)
// 0x01 = reception error (set for any of the other three)
// 0x02 = parity error
// 0x04 = overflow error (receive data lost)
// 0x08 = frame error (line corruption or incorrect line format)
int rs232raw_geterrs(int rawCRU) {
    int ret;

    __asm__ (
        "mov %1,r12\n\tai r12,18\n\tclr %0\n\tstcr %0,4\n\tswpb %0" : "=rm" (ret) : "r" (rawCRU) : "r12" 
    );

    return ret;
}
