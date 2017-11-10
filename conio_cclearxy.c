#include "conio.h"

void cclearxy(int col, int row, int v) {
    gotoxy(col,row); 
    cclear(v);
}

