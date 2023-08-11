// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// test if a byte is available at the specified serial port (returns 0 if not, other value if so)
int rs232_poll(int card, int uart) {
    int ret;
    int rawCRU = rs232raw_getuart(card, uart);
    ret = rs232raw_poll(rawCRU);
    rs232raw_deactivateCard(card);

    return ret;
}
