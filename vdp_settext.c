#include "vdp.h"

int set_text() {
	// this layout is untested but based on tested code
	int unblank = VDP_MODE1_16K | VDP_MODE1_UNBLANK | VDP_MODE1_TEXT | VDP_MODE1_INT;
	VDP_SET_REGISTER(VDP_REG_MODE0, 0);
	VDP_SET_REGISTER(VDP_REG_MODE1, VDP_MODE1_16K | VDP_MODE1_TEXT);
	VDP_SET_REGISTER(VDP_REG_SIT, 0x00);	gImage = 0x000;
	VDP_SET_REGISTER(VDP_REG_PDT, 0x01);	gPattern = 0x800;
	// no sprites and no color in text mode anyway
	nTextRow = 920;
	nTextEnd = 959;
	nTextPos = nTextRow;
	return unblank;
}
