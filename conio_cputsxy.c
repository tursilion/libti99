#include "conio.h"

void cputsxy(iny xx, int yy, const char *s) {
    gotoxy(xx, yy);
    cputs(s);
}

