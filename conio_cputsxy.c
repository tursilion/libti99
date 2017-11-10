#include "conio.h"

void cputsxy(int xx, int yy, const char *s) {
    gotoxy(xx, yy);
    cputs(s);
}

