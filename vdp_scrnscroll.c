#include "vdp.h"

void scrn_scroll_default() {
	// hacky, slow, but functional scroll that takes minimal memory
	unsigned char x[4];		// 4 byte buffer to speed it up
	int nLine = nTextEnd-nTextRow+1;
	for (int adr=nLine+gImage; adr<nTextEnd; adr+=4) {
		vdpmemread(adr, x, 4);
		vdpmemcpy(adr-nLine, x, 4);
	}
	vdpmemset(nTextRow, ' ', nLine);	// clear the last line

    // color attribute table is only at these addresses in 80 column color modes.
	if (gColor == 0x800 || gColor == 0x1800) {
		// copy color attributes the same way
		extern unsigned int conio_scrnCol; // conio_bgcolor.c

		for (int adr=nLine+gColor; adr<nTextEnd+gColor; adr+=4) {
			vdpmemread(adr, x, 4);
			vdpmemcpy(adr-nLine, x, 4);
		}
		vdpmemset(nTextRow+gColor, conio_scrnCol, nLine);	// clear the last line
	}
}

void (*scrn_scroll)() = scrn_scroll_default;
