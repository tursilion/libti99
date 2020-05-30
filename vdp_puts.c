#include "vdp.h"

// some text output code (printable characters only)
// with very limited formatting, \r, \n
// C standard version outputs a carriage return
int puts(char *s) {
    putstring(s);
    putchar('\n');
    return 1;   // non-negative number on success
}
