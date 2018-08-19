// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "rs232.h"

// activates the card, and calculates and returns the correct uart rawCRU address for
// the rest of the raw functions. You MUST call deactivateCard when done!
int rs232raw_getuart(int card, int uart) {
    rs232raw_activateCard(card);
    return card+uart;
}
