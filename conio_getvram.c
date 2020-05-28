#include "conio.h"

int conio_x=0,conio_y=0;

// get a VRAM address based on the screen mode we're in
unsigned int conio_getvram() {
    if (nTextEnd==959) {
        return VDP_SCREEN_TEXT(conio_y,conio_x)+gImage;
    } else if (nTextEnd==1919 || nTextRow == 80*29) {
        return VDP_SCREEN_TEXT80(conio_y,conio_x)+gImage;
    } else if (nTextRow == 64*23) {
        return VDP_SCREEN_TEXT64(conio_y,conio_x)+gImage;
    } else {
        return VDP_SCREEN_POS(conio_y,conio_x)+gImage;
    }
}

