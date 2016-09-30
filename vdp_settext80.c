#include "vdp.h"

int set_text80() {
	// this layout is untested but based on tested code
	int unblank = VDP_MODE1_16K | VDP_MODE1_UNBLANK | VDP_MODE1_TEXT | VDP_MODE1_INT;
	VDP_SET_REGISTER(VDP_REG_MODE0, VDP_MODE0_80COL);
	VDP_SET_REGISTER(VDP_REG_MODE1, VDP_MODE1_16K | VDP_MODE1_TEXT);
	VDP_SET_REGISTER(VDP_REG_SIT, 0x00);	gImage = 0x000;
	VDP_SET_REGISTER(VDP_REG_PDT, 0x02);	gPattern = 0x1000;
	// no sprites and no color in text mode anyway
	nTextRow = 80 * 23;
	nTextEnd = (80 * 24) - 1;
	nTextPos = nTextRow;
	return unblank;
}
