#include "conio.h"

int conio_x=0,conio_y=0;

// get a VRAM address based on the screen mode we're in
unsigned int conio_getvram() {
    if (nTextFlags&TEXT_WIDTH_40) {
        return VDP_SCREEN_TEXT(conio_y,conio_x)+gImage;
    } else if (nTextFlags&TEXT_WIDTH_80) {
        return VDP_SCREEN_TEXT80(conio_y,conio_x)+gImage;
    } else if (nTextFlags&TEXT_WIDTH_64) {
        return VDP_SCREEN_TEXT64(conio_y,conio_x)+gImage;
    } else {
        return VDP_SCREEN_POS(conio_y,conio_x)+gImage;
    }
}

