// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// writes a byte to the specified serial port - returns 0 if the byte could not be written
// (because the write register is full)
int rs232_writebyte(int card, int uart, int ch) {
    int rawCRU = rs232raw_getuart(card, uart);
    if (rs232raw_checkstatus(rawCRU) & 0x02) {
        rs232raw_writebyte(rawCRU, ch);
        rs232raw_deactivateCard(card);
        return 1;
    } else {
        rs232raw_deactivateCard(card);
        return 0;
    }
}

