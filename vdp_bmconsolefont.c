
#include "vdp.h"

// room for the patterns of the displayable characters
unsigned char bmFont[BM_FONT_SIZE];

void bm_consolefont() {
  // setup graphics mode and load the charset from grom
  // then copy those patterns out to RAM
  set_graphics(0);
  charset();
  vdpmemread(gPattern + (8 * 32), bmFont, BM_FONT_SIZE);
  gBmFont = bmFont;
}



