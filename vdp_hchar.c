#include "vdp.h"

void hchar(int r, int c, int ch, int cnt) {
	vdpmemset(gImage+(r<<5)+c, ch, cnt);
}
