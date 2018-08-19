// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// read a byte from the specified PIO (sets it to input)
int rs232_readPIO(int card) {
    int ret;
    rs232raw_setPIOinput(card);
    ret = PIO_PORT;
    rs232raw_deactivateCard(card);

    return ret;
}
