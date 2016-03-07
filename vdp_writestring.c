#include "vdp.h"

void writestring(int row, int col, char *pStr) {
  if (nTextEnd > 767) {
    VDP_SET_ADDRESS_WRITE(VDP_SCREEN_TEXT(row,col));
  } else {
    VDP_SET_ADDRESS_WRITE(VDP_SCREEN_POS(row,col));
  }
	while (*pStr) {
		VDPWD = *(pStr++);
	}
}
