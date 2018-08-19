// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// sets the specified line format, use the RS232_CTRL_xxx bits
void rs232_setcontrol(int card, int uart, int control) {
    int rawCRU = rs232raw_getuart(card, uart);

    __asm__ (
        "MOV %0,R12\n\tSBO 14\n\tLDCR %1,8" : : "r" (rawCRU), "r" (control) : "r12" 
    );

    rs232raw_deactivateCard(card);
}

