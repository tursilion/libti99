// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// write a byte to the specified PIO (sets it to output)
void rs232_writePIO(int card, int byte) {
    rs232raw_setPIOoutput(card);
    PIO_PORT = byte;
    rs232raw_deactivateCard(card);
}
