#include "conio.h"

void cclear(unsigned int len) {
    for (int idx=len; idx>0; --idx) { 
        cputc(' '); 
    }
}
