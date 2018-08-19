// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// check the TX and RX status
// 0x01 = receive buffer contains a byte
// 0x02 = transmit buffer empty
// 0x04 = transmission line clear
// 0x08 = timer error (not used in this code)
// 0x10 = time elapsed (not used in this code)
// 0x20 = RTS (inverted)
// 0x40 = DSR (inverted)
// 0x80 = CTS (inverted)
int rs232raw_geterrs(int rawCRU) {
    int ret;

    __asm__ (
        "mov %1,r12\n\tai r12,42\n\tclr %0\n\tstcr %0,8\n\tswpb %0" : "=rm" (ret) : "r" (rawCRU) : "r12" 
    );

    return ret;
}
