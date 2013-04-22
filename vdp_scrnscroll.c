#include "vdp.h"

void scrn_scroll() {
	// hacky, slow, but functional scroll that takes minimal memory
	unsigned char x[4];		// 4 byte buffer to speed it up
	int nLine = nTextEnd-nTextRow+1;
	for (int adr=nLine+gImage; adr<nTextEnd; adr+=4) {
		vdpmemread(adr, x, 4);
		vdpmemcpy(adr-nLine, x, 4);
	}
	vdpmemset(nTextRow, ' ', nLine);	// clear the last line
}
