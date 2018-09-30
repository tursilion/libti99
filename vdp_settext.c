#include "vdp.h"

int set_text_raw() {
	int unblank = VDP_MODE1_16K | VDP_MODE1_UNBLANK | VDP_MODE1_TEXT | VDP_MODE1_INT;
	VDP_SET_REGISTER(VDP_REG_MODE0, 0);
	VDP_SET_REGISTER(VDP_REG_MODE1, VDP_MODE1_16K | VDP_MODE1_TEXT);
	VDP_SET_REGISTER(VDP_REG_SIT, 0x00);	gImage = 0x000;
	VDP_SET_REGISTER(VDP_REG_PDT, 0x01);	gPattern = 0x800;
	// no sprites and no color in text mode anyway
	nTextRow = 920;
	nTextEnd = 959;
	nTextPos = nTextRow;
#if defined(ENABLE_F18A) || defined(ENABLE_TEXT64)
        vdpchar = vdpchar_default;
#endif
	return unblank;
}

void set_text() {
    int x = set_text_raw();
    VDP_SET_REGISTER(VDP_REG_MODE1, x);
    VDP_REG1_KSCAN_MIRROR = x;
}

