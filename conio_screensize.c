#include "conio.h"

void screensize(unsigned char *x, unsigned char *y) {
    // always 24 rows, but we need to do a little math for 32, 40 or 80 col
    *y = 24;
    *x = nTextEnd - nTextRow + 1;
}

