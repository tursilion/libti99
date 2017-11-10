#include "conio.h"

void chlinexy(int xx, int yy, int v) {
    gotoxy(xx,yy); 
    chline(v);
}
