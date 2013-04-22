// VDP code for the TI-99/4A by Tursi
// and a little GROM code too since I need it for the
// character set copy.
// You can copy this file and use it at will if it's useful

#include "vdp.h"

// some text output stuff
int nTextRow,nTextEnd;		// definitions for the bottom line of the screen
int nTextPos;
