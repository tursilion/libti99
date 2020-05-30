// VDP code for the TI-99/4A by Tursi
// You can copy this file and use it at will if it's useful

#include "vdp.h"

void fast_scrn_scroll_default();

// setting up standard screen modes - blanks the screen and returns the unblank command
// interrupts are also disabled. Unblank will re-enable them, too, write it to VDP_REG_MODE1
int set_graphics_raw(int sprite_mode) {
    vdpchar = vdpchar_default;
    scrn_scroll = scrn_scroll_default;

	int unblank = VDP_MODE1_16K | VDP_MODE1_UNBLANK | VDP_MODE1_INT | sprite_mode;
	VDP_SET_REGISTER(VDP_REG_MODE0, 0);
	VDP_SET_REGISTER(VDP_REG_MODE1, VDP_MODE1_16K);		// no need to OR in the sprite mode for now
	VDP_SET_REGISTER(VDP_REG_SIT, 0x00);	gImage = 0x000;
	VDP_SET_REGISTER(VDP_REG_CT, 0x0e);		gColor = 0x380;
	VDP_SET_REGISTER(VDP_REG_PDT, 0x01);	gPattern = 0x800;
	VDP_SET_REGISTER(VDP_REG_SAL, 0x06);	gSprite = 0x300;	vdpchar(gSprite, 0xd0);
	VDP_SET_REGISTER(VDP_REG_SDT, 0x01);	gSpritePat = 0x800;
	nTextRow = 736;
	nTextEnd = 767;
	nTextPos = nTextRow;
	return unblank;
}

void set_graphics(int sprite_mode) {
    int x = set_graphics_raw(sprite_mode);
    VDP_SET_REGISTER(VDP_REG_MODE1, x);
    VDP_REG1_KSCAN_MIRROR = x;
}
