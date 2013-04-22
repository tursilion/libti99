#include "vdp.h"

// some text output code (printable characters only)
// with very limited formatting, \r, \n
void putstring(char *s) {
	while (*s) {
		unsigned char tmp = *s;
		switch (tmp) {
		case '\r':
			nTextPos = nTextRow;
			break;

		case '\n':
			scrn_scroll();
			nTextPos = nTextRow;
			break;

		default:
			if ((tmp < 32) || (tmp >= 127)) tmp='.';
			vdpchar(nTextPos, tmp);
			nTextPos++;
			if (nTextPos > nTextEnd) {
				scrn_scroll();
				nTextPos = nTextRow;
			}
			break;
		}
		s++;
	}
}
