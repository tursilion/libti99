#include "conio.h"

void cputs(const char *s) {
    while (*s) {
        cputc(*s);
        ++s;
    }
}


