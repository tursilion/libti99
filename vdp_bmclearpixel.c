#include "vdp.h"

void bm_clearpixel(unsigned int x, unsigned int y) {
//	unsigned int addr = (8 * (x/8)) + (256 * (y/8)) + (y%8) + gPattern;
	unsigned int addr = ((x>>3)<<3) + ((y>>3)<<8) + (y&0x07) + gPattern;
  VDP_SET_ADDRESS(addr);
  unsigned char bits = VDPRD;
  bits = bits & (0xff - (0x80 >> (x%8)));
  VDP_SET_ADDRESS_WRITE(addr);
  VDPWD = bits;
}

