#include "conio.h"

void inc_row() {
    int y = 23;
    if (nTextFlags & TEXT_HEIGHT_30) {
        y = 29;
    }

    if (conio_y >= y) {
        scrn_scroll();
        conio_y = y;
    } else {
        ++conio_y;
    }
}

void cputc(int ch) {
    if (ch >= ' ') {
        vdpchar(conio_getvram(), ch);
        ++conio_x;
        if (conio_x > nTextEnd-nTextRow) {
            conio_x=0;
            inc_row();
        }
    } else {
        // handle some control codes
        switch (ch) {
            case '\b':          // backspace
                --conio_x;
                if (conio_x < 0) {
                    conio_x = nTextEnd-nTextRow+1;
                    if (conio_y > 0) --conio_y;
                }
                break;

            case '\f':          // form feed (clear screen)
                clrscr();
                break;

            case '\n':          // newline - next row and return to 0
                inc_row();
                conio_x = 0;
                break;

            case '\r':          // carriage return - return to 0 on same row
                conio_x = 0;
                break;

            case '\t':          // tab - move to next tabstop (every 4)
                conio_x++;
                while (conio_x&0x03) ++conio_x;   // every four characters is a tab
                if (conio_x > nTextEnd-nTextRow) {
                    conio_x = 0;
                    inc_row();
                }
                break;

            case '\v':          // vertical tab - next row, same x
                inc_row();
                break;
        }
    }
}

