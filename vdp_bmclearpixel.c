#include "vdp.h"

void bm_clearpixel(int x, int y) {
  unsigned int addr = (8 * (x/8)) + (256 * (y/8)) + (y%8);
  VDP_SET_ADDRESS(addr);
  unsigned char bits = VDPRD;
  bits = bits & (0xff - (0x80 >> (x%8)));
  VDP_SET_ADDRESS_WRITE(addr);
  VDPWD = bits;
}

