#include "conio.h"

// TODO: break up these functions into separate files, they are not interdependent

// cache of screen color - foreground is only applicable to text mode
unsigned int conio_scrnCol = (COLOR_WHITE<<4) | COLOR_DKBLUE;

unsigned int bgcolor(unsigned int color) {
    unsigned int ret = conio_scrnCol&0x0f;
    conio_scrnCol=(conio_scrnCol&0xf0)|(color&0x0f);
    // preserve the old behaviour - mostly for text mode
    if (!(nTextFlags & TEXT_FLAG_HAS_ATTRIBUTES)) {
        VDP_SET_REGISTER(VDP_REG_COL, conio_scrnCol);
    }
    return ret;
}

unsigned int textcolor(unsigned int color) {
    unsigned int ret = (conio_scrnCol&0xf0)>>4;
    conio_scrnCol=(conio_scrnCol&0x0F)|((color&0x0f)<<4);
    // preserve the old behaviour
    if (!(nTextFlags & TEXT_FLAG_HAS_ATTRIBUTES)) {
        VDP_SET_REGISTER(VDP_REG_COL, conio_scrnCol);
    }
    return ret;
}

// sets the border color but does NOT change the attribute background color
unsigned int bordercolor(unsigned int x) {
    unsigned int ret = conio_scrnCol&0xf0;
    VDP_SET_REGISTER(VDP_REG_COL, ret | (x&0x0f));
    return 0;
}

unsigned char reverse(unsigned char x) {
    (void)x;
    return 0;
}
