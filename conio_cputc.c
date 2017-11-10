#include "conio.h"

void cputc(int ch) {
    if (ch >= ' ') {
        cputc(ch);
    } else {
        // handle some control codes
        switch (ch) {
            case '\b':          // backspace
                --conio_x; 
                if (conio_x < 0) {
                    conio_x = nTextEnd-nTextRow+1;
                    if (conio_y > 0) --conio_y;
                }
                gotoxy(conio_x, conio_y);
                break;

            case '\f':          // form feed (clear screen)
                clrscr();
                break;

            case '\n':          // newline - next row and return to 0
                if (conio_y >= 23) {
                    scrn_scroll();
                    conio_y = 23;
                } else {
                    ++conio_y;
                }
                conio_x = 0;
                gotoxy(conio_x, conio_y);
                break;

            case '\r':          // carriage return - return to 0 on same row
                conio_x = 0;
                gotoxy(conio_x, conio_y);
                break;

            case '\t':          // tab - move to next tabstop (every 4)
                conio_x++;
                while (conio_x&0x03) ++conio_x;   // every four characters is a tab
                if (conio_x > nTextEnd-nTextRow) {
                    conio_x = 0;
                    if (conio_y >= 23) {
                        scrn_scroll();
                        conio_y = 23;
                    } else {
                        ++conio_y;
                    }
                }
                gotoxy(conio_x, conio_y);
                break;

            case '\v':          // vertical tab - next row, same x
                if (conio_y >= 23) {
                    scrn_scroll();
                    conio_y = 23;
                } else {
                    ++conio_y;
                }
                gotoxy(conio_x, conio_y);
                break;
        }
    }
}

