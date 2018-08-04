
#include "vdp.h"

unsigned char gBitmapColor = 0;

void bm_setforeground(int c) {
  gBitmapColor = (gBitmapColor & 0x0F) | (c << 4);
}

void bm_setbackground(int c) {
  gBitmapColor = (gBitmapColor & 0xF0) | (c & 0x0F);
}

