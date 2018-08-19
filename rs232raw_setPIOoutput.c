// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// activates the card and sets PIO to output - you must use deactivateCard when finished!
void rs232raw_setPIOoutput(int card) {
    rs232raw_activateCard(card);
    __asm__ ( "MOV %0,R12\n\tSBZ 1" : : "r" (card) : "r12" );
}
