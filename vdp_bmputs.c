
#include "vdp.h"

unsigned char* gBmFont = 0;

// limited to 32 x 24 character placement grid.
void bm_placetile(int x, int y, const unsigned char* pattern) {
  unsigned int blockAddr = gPattern + (x * 8) + (y * 8 * 32);
  vdpmemcpy(blockAddr, pattern, 8);
  vdpmemset(gColor + blockAddr, gBitmapColor, 8);
}

// assumes font is ' ' through '~'
void bm_putc(int x, int y, unsigned char alphanum) {
  unsigned char* fontPattern = (unsigned char*) gBmFont + (((int) alphanum - 32) * 8);
  bm_placetile(x, y, fontPattern);
}

void bm_puts(int x, int y, unsigned char* str) {
  unsigned char* cursor = str;
  while(*cursor != 0) {
    bm_putc(x++, y, *cursor++);
  }
}

