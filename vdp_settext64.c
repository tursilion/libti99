#include "vdp.h"

#ifdef ENABLE_TEXT64
int text64_scroll = 0;

int set_text64_raw(void) {
	// note: no masking, full size bitmap mode
	int unblank = VDP_MODE1_16K | VDP_MODE1_UNBLANK | VDP_MODE1_INT;
	VDP_SET_REGISTER(VDP_REG_MODE0, VDP_MODE0_BITMAP);
	VDP_SET_REGISTER(VDP_REG_MODE1, VDP_MODE1_16K);		// no need to OR in the sprite mode for now
	VDP_SET_REGISTER(VDP_REG_SIT, 0x0E);	gImage = 0x3800;
	VDP_SET_REGISTER(VDP_REG_CT, 0xFF);		gColor = 0x2000;
	VDP_SET_REGISTER(VDP_REG_PDT, 0x03);	gPattern = 0x0000;
	VDP_SET_REGISTER(VDP_REG_SAL, 0x76);	gSprite = 0x3B00;	vdpchar(gSprite, 0xd0);
	VDP_SET_REGISTER(VDP_REG_SDT, 0x03);	gSpritePat = 0x1800;
	nTextRow = 64*23;
	nTextEnd = 64*24-1;
	nTextPos = nTextRow;

	int i;
	for (i = 0; i < 32*24; i++) {
            VDP_SET_ADDRESS_WRITE(gImage + i);
            VDPWD=i;
	}


	return unblank;
}

void set_text64_color(void) {
    int x = set_text64_raw();
    VDP_SET_REGISTER(VDP_REG_MODE1, x);
    VDP_REG1_KSCAN_MIRROR = x;
}

#endif