#include "conio.h"

void cputcxy(int xx, int yy, int ch) {
    gotoxy(xx,yy); 
    cputc(ch);
}
