// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// set the bitrate on the card and uart (sets both directions the same)
void rs232_setbps(int card, int uart, int bps) {
    int rawCRU = rs232raw_getuart(card, uart);

    __asm__ (
        "  mov %0,r12\n"	// get the rawcru address
        "  sbo 12\n"		// request write to recv rate
        "  ldcr %1,11\n"	// load recv rate (auto-decrements to send rate)
        "  ldcr %1,11\n"	// load send rate (Nouspikel says you can do both at once, but safer this way)
        : : "r" (rawCRU), "r" (bps) : "r12" 
    );

    rs232raw_deactivateCard(card);
}
