#include "conio.h"

void screensize(unsigned char *x, unsigned char *y) {
    // always 23 rows, but we need to do a little math for 32, 40 or 80 col
    *y = 23;
    *x = nTextEnd - nTextRow + 1;
}

