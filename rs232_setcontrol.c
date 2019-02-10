// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// sets the specified line format, use the RS232_CTRL_xxx bits
void rs232_setcontrol(int card, int uart, int control) {
    int rawCRU = rs232raw_getuart(card, uart);

	// Note: the reset is supposed to set the bits for loading:
	// control register(14), interval register (13), rx rate (12)
	// and tx rate (11). The highest one set is the one that is
	// loaded. But in order to allow more flexibility with this
	// library, as well as to deal with incomplete emulation and
	// potential new devices that also emulate a 9902, we'll just
	// be very selective and not make assumptions!

    __asm__ (
        "  mov %0,r12\n"	// get the rawcru address
        "  sbo 31\n"		// reset
        "  sbz 21\n"		// rts/cts interrupts off
        "  sbz 20\n"		// timer ints off
        "  sbz 19\n"		// tx int off
        "  sbz 18\n"		// rx int off
        "  sbz 17\n"		// clear abort/BREAK
        "  sbz 15\n"		// clear loopback test mode
        "  sbo 14\n"		// request to write control word
        "  sbz 13\n"		// not request to write interval register
        "  sbz 12\n"		// not request to write rx data rate
        "  sbz 11\n"		// not request to write tx data rate
        "  swpb %1\n"		// get the byte into the msb
        "  ldcr %1,8\n"		// write it
        "  sbz 14\n"		// and turn it off for later use
        "  swpb %1\n"		// fix the reg in case gcc wants it
        
        : : "r"(rawCRU), "r"(control) : "r12" 
    );

    rs232raw_deactivateCard(card);
}

