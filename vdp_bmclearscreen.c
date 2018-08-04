
#include "vdp.h"

void bm_clearscreen() {
  vdpwriteinc(gImage,0,768);
  vdpmemset(gColor,gBitmapColor,768*8);
  vdpmemset(gPattern,0,768*8);
}

