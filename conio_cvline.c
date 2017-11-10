#include "conio.h"

void cvline(int len) {
    int orig_conio_y = conio_y;
    int end = conio_y+len;
    for (; conio_y<end; ++conio_y) {
        if (conio_y > 23) break;
        vdpchar(conio_getvram(), '|');
    }
    gotoxy(conio_x, orig_conio_y);
}
