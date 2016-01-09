#include "vdp.h"

// write a character with end of line detection
int putchar(int x) {
  if (x == '\n') {
    scrn_scroll();
    nTextPos = nTextRow;
  } else if (x == '\r') {
		nTextPos = nTextRow;
  } else {
    vdpchar(nTextPos, x);
    ++nTextPos;
    if (nTextPos > nTextEnd) {
      scrn_scroll();
      nTextPos = nTextRow;
    }
  }
  return x;
}

