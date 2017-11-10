#include "conio.h"

void cputcxy(iny xx, int yy, int ch) {
    gotoxy(xx,yy); 
    cputc(ch);
}
