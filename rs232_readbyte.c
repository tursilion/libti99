// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// reads a byte from the specified serial port, blocks - so always check rs232_poll first!
int rs232_readbyte(int card, int uart) {
    int ret;
    int rawCRU = rs232raw_getuart(card, uart);

    // infinite loop, so poll before you call me!
    while (!rs232raw_poll(rawCRU)) { }
    ret = rs232raw_readbyte(rawCRU);

    rs232raw_deactivateCard(card);

    return ret;
}
