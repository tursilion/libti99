// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// set the bitrate on the card and uart (sets both directions the same)
void rs232_setbps(int card, int uart, int bps) {
    int rawCRU = rs232raw_getuart(card, uart);

	// you don't technically have to turn off the bits after a write - the
	// chip is supposed to turn them off for you. But again, I'm thinking
	// about buggy emulation and broken future chips. ;)

    __asm__ (
        "  mov %0,r12\n"	// get the rawcru address
        "  sbz 14\n"		// not request to write control word
        "  sbz 13\n"		// not request to write interval register
        "  sbo 12\n"		// request write to rx rate
        "  sbz 11\n"		// not request to write tx data rate
        "  ldcr %1,11\n"	// load rx rate (auto-decrements to send rate)
        "  sbz 12\n"		// not request write to rx rate
        "  sbo 11\n"		// request to write tx data rate
        "  ldcr %1,11\n"	// load tx rate (Nouspikel says you can do both at once, but safer this way)
        "  sbz 11\n"		// and turn it off too
        : : "r" (rawCRU), "r" (bps) : "r12" 
    );

    rs232raw_deactivateCard(card);
}
