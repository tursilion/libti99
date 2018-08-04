#include "vdp.h"

void bm_setpixel(int x, int y) {
  unsigned int addr = (8 * (x/8)) + (256 * (y/8)) + (y%8);
  VDP_SET_ADDRESS(addr);
  unsigned char bits = VDPRD;
  bits = bits | (0x80 >> (x%8));
  VDP_SET_ADDRESS_WRITE(addr);
  VDPWD = bits;
  VDP_SET_ADDRESS_WRITE(gColor + addr);
  VDPWD = gBitmapColor;
}

