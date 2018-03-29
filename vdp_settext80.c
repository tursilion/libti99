#include "vdp.h"

// requires F18A or 9938!!
int set_text80_raw() {
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

void set_text80() {
    int x = set_text80_raw();
    VDP_SET_REGISTER(VDP_REG_MODE1, x);
    VDP_REG1_KSCAN_MIRROR = x;
}

#ifdef ENABLE_F18A
void set_text80_color(void)
{
    // unlock the F18A (should be done before setting the mode)
    VDP_SET_REGISTER(0x39, 0x1c);  // Write once
    VDP_SET_REGISTER(0x39, 0x1c);  // Write twice, unlock

    int x = set_text80_raw();
    VDP_SET_REGISTER(VDP_REG_MODE1, x);
    VDP_REG1_KSCAN_MIRROR = x;

    VDP_SET_REGISTER(VDP_REG_CT, 0x20);		gColor = 0x800;
    // sprites are active when F18A is unlocked
    VDP_SET_REGISTER(VDP_REG_SAL, 0x1800/0x80); gSprite = 0x1800;

    extern unsigned int conio_scrnCol; // conio_bgcolor.c
    vdpmemset(gColor, conio_scrnCol, nTextEnd+1);	// clear the color table

    VDP_SET_REGISTER(0x32, 0x02);  // set Position-based tile attributes
}
#endif
