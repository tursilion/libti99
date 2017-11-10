#include "conio.h"

void cvlinexy(int x, int y, int len) {
    gotoxy(x,y);
    cvline(len);
}
