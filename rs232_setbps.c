// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// set the bitrate on the card and uart (sets both directions the same)
void rs232_setbps(int card, int uart, int bps) {
    int rawCRU = rs232raw_getuart(card, uart);

    __asm__ (
        "MOV %0,R12\n\tSBO 11\n\tSBO 12\n\tLDCR %1,11" : : "r" (rawCRU), "r" (bps) : "r12" 
    );

    rs232raw_deactivateCard(card);
}
