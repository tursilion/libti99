#include "conio.h"

// cache of screen color - foreground is only applicable to text mode
unsigned int conio_scrnCol = (COLOR_WHITE<<4) | COLOR_DKBLUE;

unsigned int bgcolor(unsigned int color) {
    unsigned int ret = conio_scrnCol&0x0f;
    conio_scrnCol=(conio_scrnCol&0xf0)|(color&0x0f); 
    VDP_SET_REGISTER(VDP_REG_COL, conio_scrnCol);
    return ret;
}

unsigned int textcolor(unsigned int color) {
    unsigned int ret = (conio_scrnCol&0xf0)>>4;
    conio_scrnCol=(conio_scrnCol&0x0F)|((color&0x0f)<<4); 
    VDP_SET_REGISTER(VDP_REG_COL, conio_scrnCol);
    return ret;
}
