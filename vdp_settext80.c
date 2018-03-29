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

void gpu_scroll(void)
{
     __asm__(
// copy code starting at loop0 to VDP address >4000
	"li r0,>3f16\n"  // source
"	li r1,>4000\n"   // dest
"       li r2,>34/2\n" // count
"loopx\n"
"	mov *r0+,*r1+\n"
"	dec r2\n"
"       jne loopx\n"
"       b @>4000\n"

// move 23 rows of chars up
"loop0\n"
"	li r0,80\n"
"	clr r1\n"
"	li r2,80*23/2\n"
"loop1\n"
"	mov *r0+,*r1+\n"
"	dec r2\n"
"	jne loop1\n"
// fill bottom row with spaces
"	li r0,>2020\n"
"       li r2,80/2\n"
"loop1a\n"
"	mov r0,*r1+\n"
"	dec r2\n"
"	jne loop1a\n"
// move 23 rows of colors up
"	li r0,0x800+80\n"
"	li r1,0x800\n"
"	li r2,80*23/2\n"
"loop2\n"
"	mov *r0+,*r1+\n"
"	dec r2\n"
"	jne loop2\n"
// stop gpu and restart when triggered
"	idle\n"
"	jmp loop0\n"
     :::
     "r0","r1","r2"
     );
}


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
    VDP_SET_REGISTER(VDP_REG_SAL, 0x1800/0x80); gSprite = 0x1800; vdpchar(gSprite, 0xd0);

    extern unsigned int conio_scrnCol; // conio_bgcolor.c
    vdpmemset(gColor, conio_scrnCol, nTextEnd+1);	// clear the color table

    VDP_SET_REGISTER(0x32, 0x02);  // set Position-based tile attributes

    // load GPU scroll function
    vdpmemcpy(0x3f00, (unsigned char*)gpu_scroll, 0x4a);
    VDP_SET_REGISTER(0x36, 0x3f);
    VDP_SET_REGISTER(0x37, 0x00);

}
#endif
