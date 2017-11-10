#include "conio.h"
#include <stdarg.h>
#include "string.h"

char conio_sprintfbuf[256];

// helper function to put the string out - string will be modified and is
// expected to be 256 chars in length
int fmt_print(char *s, int zero, int width, int left, int precis, int plus) {
    int cnt = 0;
    int i = strlen(s);
    int pad = ' ';
    if (zero) pad = '0';

    if ((precis > 0) && (i >= precis)) {
        i = precis;
        if (plus) {
            --i;
            if (width > 0) --width;
        }
    }
    if (width <= 0) {
        if (plus) {
            cputc('+');
            ++cnt;
        }
        for (int idx=0; idx<i; ++idx) {
            cputc(*(s++));
            ++cnt;
        }
    } else {
        if (left) {
            if (plus) {
                cputc('+');
                ++cnt;
            }
            for (int idx=0; idx<i; ++idx) {
                cputc(*(s++));
                ++cnt;
            }
            for (int idx=0; idx<width-i; ++idx) {
                cputc(pad);
                ++cnt;
            }
        } else {
            if ((plus)&&(zero)) {
                cputc('+');
                plus = 0;
                ++cnt;
            }
            for (int idx=0; idx<width-i; ++idx) {
                cputc(pad);
                ++cnt;
            }
            if (plus) {
                cputc('+');
                ++cnt;
            }
            for (int idx=0; idx<i; ++idx) {
                cputc(*(s++));
                ++cnt;
            }
        }
    }
    return cnt;
}

// somewhat standardish printf. 
// no floats today
int cprintf(const char *fmt, ...) {
    int i=0;
    unsigned int u=0;
    char *s=0;

    va_list argp;
    va_start(argp, fmt);
    return vcprintf(fmt, argp);
}

int vcprintf(const char *fmt, va_list argp) {
    int cnt = 0;

    while (*fmt) {
        if (*fmt != '%') {
            cputc(*fmt);
        } else {
            ++fmt;
            int zero = 0;
            int width = 0;
            int left = 0;
            int plus = 0;
            int ucase = 0;
            int precis = 0;
            int done = 0;
            while (!done) {
                switch (*fmt) {
                    case '-': left = 1; break;
                    case '+': plus = 1; break;
                    case '.': precis = -1; break;
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': 
                        if (precis) {
                            if (precis == -1) {
                                precis = (*fmt)-'0';
                            } else {
                                precis = (precis*10) + ((*fmt)-'0');
                            }
                        } else {
                            if (width == 0) { 
                                if (*fmt == '0') {
                                    // this i a zero pad specifier
                                    zero=1;
                                } else {
                                    width = (*fmt)-'0';
                                }
                            } else {
                                width = (width*10) + ((*fmt)-'0');
                            }
                        }
                        break;

                    case 'c':   // char
                       i = va_arg(argp, int);
                       conio_sprintfbuf[0]=i;
                       conio_sprintfbuf[1]='\0';
                       cnt += fmt_print(conio_sprintfbuf, 0, width, left, precis, 0);
                       done = 1;
                       break;

                    case 'i':   // decimal
                    case 'd':   // decimal
                        i = va_arg(argp, int);
                        s = int2str(i);
                        strcpy(conio_sprintfbuf, s);
                        cnt += fmt_print(conio_sprintbuf, zero, width, left, precis, plus);
                        done = 1;
                        break;

                    case 'u':   // unsigned decimal
                        u = va_arg(argp, unsigned int);
                        s = uint2str(u);
                        strcpy(conio_sprintfbuf, s);
                        cnt += fmt_print(conio_sprintbuf, zero, width, left, precis, plus);
                        done = 1;
                        break;

                    case 's':   // string
                        s = va_arg(argp, char*);
                        cnt += fmt_print(s, 0, width, left, precis, 0);
                        done = 1;
                        break;

                    case 'X':   // uppercase hex
                        ucase = 1;
                        // fall through

                    case 'x':   // hex (a little inefficient..)
                        u = va_arg(argp, unsigned int);
                        i = (u&0xf000)>>12;
                        if (i>9) i+=7;
                        conio_sprintfbuf[0]=i+'0';
                        i = (u&0xf00)>>8;
                        if (i>9) i+=7;
                        conio_sprintfbuf[1]=i+'0';
                        i = (u&0xf0)>>4;
                        if (i>9) i+=7;
                        conio_sprintfbuf[2]=i+'0';
                        i = (u&0xf);
                        if (i>9) i+=7;
                        conio_sprintfbuf[3]=i+'0';
                        conio_sprintfbuf[4]='\0';
                        if (!zero) {
                            // remove leading zeros
                            while (conio_sprintfbuf=='0') {
                                // my memcpy is safe in this direction only...
                                memcpy(&conio_sprintfbuf[0], &conio_sprintfbuf[1], 4);  // includes NUL
                            }
                            if (conio_sprintfbuf[0] == '\0') {
                                conio_sprintfbuf[0]='0';
                                conio_sprintfbuf[1]='\0';
                            }
                        }
                        if (!ucase) {
                            // make lowercase
                            for (int idx=0; idx<4; ++idx) {
                                if ((conio_sprintfbuf[idx]>='A')&&(conio_sprintfbuf[idx]<='F')) {
                                    conio_sprintfbuf[idx]+=32;  // make lowercase
                                }
                            }
                        }
                        cnt += fmt_print(conio_sprintfbuf, 0, width, left, precis, 0);
                        done = 1;
                        break;

                    case 'o':   // octal (roughly same as hex)
                        u = va_arg(argp, unsigned int);
                        i = (u&0x8000)>>15;
                        conio_sprintfbuf[0]=i+'0';
                        i = (u&0x7000)>>12;
                        conio_sprintfbuf[1]=i+'0';
                        i = (u&0x0e00)>>9;
                        conio_sprintfbuf[2]=i+'0';
                        i = (u&0x01c0)>>6;
                        conio_sprintfbuf[3]=i+'0';
                        i = (u&0x0038)>>3;
                        conio_sprintfbuf[4]=i+'0';
                        i = (u&0x0007);
                        conio_sprintfbuf[5]=i+'0';
                        conio_sprintfbuf[6]='\0';
                        if (!zero) {
                            // remove leading zeros
                            while (conio_sprintfbuf=='0') {
                                // my memcpy is safe in this direction only...
                                memcpy(&conio_sprintfbuf[0], &conio_sprintfbuf[1], 6);  // includes NUL
                            }
                            if (conio_sprintfbuf[0] == '\0') {
                                conio_sprintfbuf[0]='0';
                                conio_sprintfbuf[1]='\0';
                            }
                        }
                        cnt += fmt_print(conio_sprintfbuf, 0, width, left, precis, 0);
                        done = 1;
                        break;

                    case '%':   // percent sign
                        cputc('%');
                        ++cnt;
                        done = 1;
                        break;

                    // probably useful...
                    //case 'e':   // exp float (no support)
                    //case 'f':   // float (no support)

                    default:
                        cputc(*fmt);
                        ++cnt;
                        done = 1;
                        break;
                }
                ++fmt;
            }
        }
        ++fmt;
    }

    return cnt;
}

