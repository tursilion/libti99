#include "conio.h"

void clrscr() {
    gotoxy(0,0); 
    vdpmemset(conio_getvram(), ' ', nTextEnd+1);
}
