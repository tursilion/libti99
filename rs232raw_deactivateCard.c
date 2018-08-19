// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// turns off the card and the card LED
void rs232raw_deactivateCard(int card) {
    __asm__ (
        "mov %0,r12\n\tsbz 7\n\rsbz 0" : : "r" (card) : "r12" 
    );
}
