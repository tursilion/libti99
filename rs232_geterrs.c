// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// return the error bits from the specified port (we don't check the timer here)
// 0x01 = reception error (set for any of the other three)
// 0x02 = parity error
// 0x04 = overflow error (receive data lost)
// 0x08 = frame error (line corruption or incorrect line format)
int rs232_geterrs(int card, int uart) {
    int ret;
    
    int rawCRU = rs232raw_getuart(card, uart);
    ret = rs232raw_geterrs(rawCRU);
    rs232raw_deactivateCard(card);

    return ret;
}
