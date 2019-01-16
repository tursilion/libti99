#include "conio.h"

// cache of screen color - foreground is only applicable to text mode
unsigned int conio_scrnCol = (COLOR_WHITE<<4) | COLOR_DKBLUE;

unsigned int bgcolor(unsigned int color) {
    unsigned int ret = conio_scrnCol&0x0f;
    conio_scrnCol=(conio_scrnCol&0xf0)|(color&0x0f);
    if (gColor == 0x800 || nTextRow == 64*23)
        return ret;
    VDP_SET_REGISTER(VDP_REG_COL, conio_scrnCol);
    return ret;
}

unsigned int textcolor(unsigned int color) {
    unsigned int ret = (conio_scrnCol&0xf0)>>4;
    conio_scrnCol=(conio_scrnCol&0x0F)|((color&0x0f)<<4); 
    if (gColor == 0x800 || nTextRow == 64*23)
        return ret;
    VDP_SET_REGISTER(VDP_REG_COL, conio_scrnCol);
    return ret;
}

unsigned int bordercolor(unsigned int x) { 
    (void)x; 
    return 0;
}

unsigned char reverse(unsigned char x) { 
    (void)x;  
    return 0; 
}
