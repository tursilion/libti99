#include "vdp.h"

int text64_scroll = 0;

static void fast_scrn_scroll64();

static void vdpchar64(int pAddr, int ch);

int set_text64_raw(void) {
    extern unsigned int conio_scrnCol; // conio_bgcolor.c

	// note: no masking, full size bitmap mode
	int unblank = VDP_MODE1_16K | VDP_MODE1_UNBLANK | VDP_MODE1_INT;

	vdpchar = vdpchar64;
	scrn_scroll = fast_scrn_scroll64;

	VDP_SET_REGISTER(VDP_REG_MODE0, VDP_MODE0_BITMAP);
	VDP_SET_REGISTER(VDP_REG_MODE1, VDP_MODE1_16K);		// no need to OR in the sprite mode for now
	VDP_SET_REGISTER(VDP_REG_SIT, 0x0E);	gImage = 0x3800;
	VDP_SET_REGISTER(VDP_REG_CT, 0xFF);		gColor = 0x2000;
	VDP_SET_REGISTER(VDP_REG_PDT, 0x03);	gPattern = 0x0000;
	VDP_SET_REGISTER(VDP_REG_SAL, 0x76);	gSprite = 0x3B00;	vdpchar_default(gSprite, 0xd0);
	VDP_SET_REGISTER(VDP_REG_SDT, 0x03);	gSpritePat = 0x1800;
	nTextRow = 64*23;
	nTextEnd = 64*24-1;
	nTextPos = nTextRow;

	int i;
    VDP_SET_ADDRESS_WRITE(gImage);
	for (i = 0; i < 32*24; i++) {
            VDPWD = i;
	}
	vdpmemset(gPattern, 0, 32*24*8);
	vdpmemset(gColor, conio_scrnCol, 32*24*8);

	return unblank;
}

void set_text64_color(void) {
    int x = set_text64_raw();
    VDP_SET_REGISTER(VDP_REG_MODE1, x);
    VDP_REG1_KSCAN_MIRROR = x;
}

static const unsigned int font3x5[] = {
0x0000,0x0000,0x0000,0x0000, 0x44AA,0xAAAA,0xAAAA,0x4400, 0x44EE,0xEEEE,0xEEEE,0x4400, 0x0000,0xAAEE,0xEE44,0x0000,
0x0000,0x44EE,0xEE44,0x0000, 0x0000,0x44EE,0xAA44,0x4400, 0x0000,0x44EE,0xEE44,0xEE00, 0x0000,0x0044,0xEE44,0x0000,
0xEEEE,0xAA00,0x00AA,0xEEEE, 0x0000,0x0044,0xAA44,0x0000, 0xEEEE,0xEEAA,0x44AA,0xEEEE, 0x0033,0x1144,0xAAAA,0x4400,
0x0044,0xAAAA,0x44EE,0x4400, 0x0063,0x4242,0xC6C6,0x0000, 0x0066,0x5555,0xDDDD,0x3333, 0x00AA,0x44AA,0x44AA,0x0000,
0x0088,0xCCEE,0xCC88,0x0000, 0x0022,0x66EE,0x6622,0x0000, 0x0044,0xEE44,0xEE44,0x0000, 0xAAAA,0xAAAA,0x00AA,0x0000,
0x0077,0xDDDD,0x5555,0x0000, 0x6688,0x44AA,0x4422,0xCC00, 0x0000,0x0000,0xEEEE,0x0000, 0x44EE,0x4444,0xEE44,0xEE00,
0x0044,0xEE55,0x4444,0x4400, 0x0044,0x4444,0x55EE,0x4400, 0x0044,0x22FF,0x2244,0x0000, 0x0022,0x44FF,0x4422,0x0000,
0x0000,0x8888,0xEE00,0x0000, 0x0000,0xAAFF,0xAA00,0x0000, 0x0000,0x4444,0xEEEE,0x0000, 0x0000,0xEEEE,0x4444,0x0000,
0x0000,0x0000,0x0000,0x0000, 0x0044,0x4444,0x0044,0x0000, 0x00AA,0xAA00,0x0000,0x0000, 0x00AA,0xFFAA,0xFFAA,0x0000,
0x44EE,0x8844,0x22EE,0x4400, 0x0088,0x2244,0x8822,0x0000, 0x0044,0xAA44,0xAA55,0x0000, 0x0044,0x8800,0x0000,0x0000,
0x0088,0x4444,0x4488,0x0000, 0x0022,0x4444,0x4422,0x0000, 0x00AA,0x44EE,0x44AA,0x0000, 0x0000,0x44EE,0x4400,0x0000,
0x0000,0x0000,0x0044,0x8800, 0x0000,0x00EE,0x0000,0x0000, 0x0000,0x0000,0x0044,0x0000, 0x0022,0x2244,0x8888,0x0000,
0x0044,0xAAAA,0xAA44,0x0000, 0x0044,0xCC44,0x44EE,0x0000, 0x00CC,0x2244,0x88EE,0x0000, 0x00CC,0x2244,0x22CC,0x0000,
0x00AA,0xAAEE,0x2222,0x0000, 0x00EE,0x88CC,0x22CC,0x0000, 0x0066,0x88CC,0xAA44,0x0000, 0x00EE,0x2244,0x8888,0x0000,
0x0044,0xAA44,0xAA44,0x0000, 0x0044,0xAA66,0x2244,0x0000, 0x0000,0x0044,0x0044,0x0000, 0x0000,0x0044,0x0044,0x8800,
0x0022,0x4488,0x4422,0x0000, 0x0000,0xEE00,0xEE00,0x0000, 0x0088,0x4422,0x4488,0x0000, 0x44AA,0x2244,0x0044,0x0000,
0x0044,0xAAEE,0xEE88,0x4400, 0x0044,0xAAEE,0xAAAA,0x0000, 0x00CC,0xAACC,0xAACC,0x0000, 0x0066,0x8888,0x8866,0x0000,
0x00CC,0xAAAA,0xAACC,0x0000, 0x00EE,0x88CC,0x88EE,0x0000, 0x00EE,0x88CC,0x8888,0x0000, 0x0066,0x88AA,0xAA66,0x0000,
0x00AA,0xAAEE,0xAAAA,0x0000, 0x00EE,0x4444,0x44EE,0x0000, 0x0022,0x2222,0xAAEE,0x0000, 0x00AA,0xAACC,0xAAAA,0x0000,
0x0088,0x8888,0x88EE,0x0000, 0x00AA,0xEEEE,0xAAAA,0x0000, 0x00CC,0xAAAA,0xAAAA,0x0000, 0x0066,0xAAAA,0xAACC,0x0000,
0x00CC,0xAACC,0x8888,0x0000, 0x0066,0xAAAA,0xAACC,0x2200, 0x00CC,0xAACC,0xAAAA,0x0000, 0x0066,0x8844,0x22CC,0x0000,
0x00EE,0x4444,0x4444,0x0000, 0x00AA,0xAAAA,0xAAEE,0x0000, 0x00AA,0xAAAA,0x4444,0x0000, 0x00AA,0xAAEE,0xEEAA,0x0000,
0x00AA,0xAA44,0xAAAA,0x0000, 0x00AA,0xAA44,0x4444,0x0000, 0x00EE,0x2244,0x88EE,0x0000, 0x0066,0x4444,0x4466,0x0000,
0x0088,0x8844,0x2222,0x0000, 0x00CC,0x4444,0x44CC,0x0000, 0x0044,0xAA00,0x0000,0x0000, 0x0000,0x0000,0x00EE,0x0000,
0x0044,0x2200,0x0000,0x0000, 0x0000,0x66AA,0xAA66,0x0000, 0x8888,0xCCAA,0xAACC,0x0000, 0x0000,0x6688,0x8866,0x0000,
0x2222,0x66AA,0xAA66,0x0000, 0x0000,0x66AA,0xCC66,0x0000, 0x6644,0xEE44,0x4444,0x0000, 0x0000,0x66AA,0xAA66,0x22CC,
0x8888,0xEEAA,0xAAAA,0x0000, 0x4400,0x4444,0x4444,0x0000, 0x2200,0x2222,0x22AA,0x4400, 0x8888,0xAACC,0xCCAA,0x0000,
0x4444,0x4444,0x4444,0x0000, 0x0000,0xEEEE,0xAAAA,0x0000, 0x0000,0xCCAA,0xAAAA,0x0000, 0x0000,0x44AA,0xAA44,0x0000,
0x0000,0xCCAA,0xAACC,0x8888, 0x0000,0x66AA,0xAA66,0x2222, 0x0000,0xAACC,0x8888,0x0000, 0x0000,0x66CC,0x22CC,0x0000,
0x0044,0xEE44,0x4444,0x0000, 0x0000,0xAAAA,0xAAEE,0x0000, 0x0000,0xAAAA,0xAA44,0x0000, 0x0000,0xAAAA,0xEEEE,0x0000,
0x0000,0xAA44,0x44AA,0x0000, 0x0000,0xAAAA,0xAA66,0x22CC, 0x0000,0xEE66,0x88EE,0x0000, 0x0066,0x4488,0x4466,0x0000,
0x0044,0x4400,0x4444,0x0000, 0x00CC,0x4422,0x44CC,0x0000, 0x0066,0xCC00,0x0000,0x0000, 0x0000,0x44AA,0xEE00,0x0000,
0x6688,0x8888,0x6622,0x6600, 0xAA00,0xAAAA,0xAAEE,0x0000, 0x2244,0x66AA,0xCC66,0x0000, 0x44AA,0x66AA,0xAA66,0x0000,
0xAA00,0x66AA,0xAA66,0x0000, 0x8844,0x66AA,0xAA66,0x0000, 0x4400,0x66AA,0xAA66,0x0000, 0x0000,0x6688,0x8866,0x2266,
0x44AA,0x66AA,0xCC66,0x0000, 0xAA00,0x66AA,0xCC66,0x0000, 0x8844,0x66AA,0xCC66,0x0000, 0xAA00,0x4444,0x4444,0x0000,
0x44AA,0x0044,0x4444,0x0000, 0x8844,0x0044,0x4444,0x0000, 0xAA44,0xAAEE,0xAAAA,0x0000, 0x44AA,0x44AA,0xEEAA,0x0000,
0x44EE,0x88CC,0x88EE,0x0000, 0x0000,0x77BB,0xAA77,0x0000, 0x0077,0xAAFF,0xAABB,0x0000, 0x44AA,0x44AA,0xAA44,0x0000,
0xAA00,0x44AA,0xAA44,0x0000, 0x8844,0x44AA,0xAA44,0x0000, 0x44AA,0x00AA,0xAAEE,0x0000, 0x8844,0x00AA,0xAAEE,0x0000,
0xAA00,0xAAAA,0xAA66,0x22CC, 0xAA00,0x66AA,0xAACC,0x0000, 0xAA00,0xAAAA,0xAAAA,0xEE00, 0x0044,0xEE88,0xEE44,0x0000,
0x2244,0xEE44,0xCC66,0x0000, 0xAAAA,0x44EE,0x44EE,0x4400, 0xCCAA,0xCCAA,0xFFAA,0x0000, 0x6644,0xEE44,0x4444,0x8800,
0x2244,0x66AA,0xAA66,0x0000, 0x2244,0x0044,0x4444,0x0000, 0x2244,0x44AA,0xAA44,0x0000, 0x2244,0x00AA,0xAAEE,0x0000,
0x66CC,0x00CC,0xAAAA,0x0000, 0xEE00,0xCCAA,0xAAAA,0x0000, 0x66AA,0x6600,0x0000,0x0000, 0x44AA,0x4400,0x0000,0x0000,
0x4400,0x4488,0xAA44,0x0000, 0x0000,0x00EE,0x8800,0x0000, 0x0000,0x00EE,0x2200,0x0000, 0x8888,0xAA44,0xBB11,0x2233,
0x8888,0xAA44,0x8855,0x7711, 0x0044,0x0044,0x4444,0x0000, 0x0000,0x55AA,0x5500,0x0000, 0x0000,0xAA55,0xAA00,0x0000,
0x0055,0x00AA,0x0055,0x00AA, 0x55AA,0x55AA,0x55AA,0x55AA, 0xFFAA,0xFF55,0xFFAA,0xFF55, 0x4444,0x4444,0x4444,0x4444,
0x4444,0x44CC,0x4444,0x4444, 0x4444,0xCC44,0xCC44,0x4444, 0xAAAA,0xAAAA,0xAAAA,0xAAAA, 0x0000,0x00EE,0xAAAA,0xAAAA,
0x0000,0xCC44,0xCC44,0x4444, 0xAAAA,0xAA22,0xAAAA,0xAAAA, 0xAAAA,0xAAAA,0xAAAA,0xAAAA, 0x0000,0xEE22,0xAAAA,0xAAAA,
0xAAAA,0xAA22,0xEE00,0x0000, 0xAAAA,0xAAEE,0x0000,0x0000, 0x4444,0xCC44,0xCC00,0x0000, 0x0000,0x00CC,0x4444,0x4444,
0x4444,0x4477,0x0000,0x0000, 0x4444,0x44FF,0x0000,0x0000, 0x0000,0x00FF,0x4444,0x4444, 0x4444,0x4477,0x4444,0x4444,
0x0000,0x00FF,0x0000,0x0000, 0x4444,0x44FF,0x4444,0x4444, 0x4444,0x7744,0x7700,0x0000, 0xAAAA,0xAABB,0xAAAA,0xAAAA,
0xAAAA,0xBB88,0xFF00,0x0000, 0x0000,0xFF88,0xBBAA,0xAAAA, 0xAAAA,0xBB00,0xFF00,0x0000, 0x0000,0xFF00,0xBBAA,0xAAAA,
0xAAAA,0xBB88,0xBBAA,0xAAAA, 0x0000,0xFF00,0xFF00,0x0000, 0xAAAA,0xBB00,0xBBAA,0xAAAA, 0x4444,0xFF00,0xFF00,0x0000,
0xAAAA,0xAAFF,0x0000,0x0000, 0x0000,0xFF00,0xFF44,0x4444, 0x0000,0x00FF,0xAAAA,0xAAAA, 0xAAAA,0xAAFF,0x0000,0x0000,
0x4444,0x7744,0x7700,0x0000, 0x0000,0x7744,0x7744,0x4444, 0x0000,0x00FF,0xAAAA,0xAAAA, 0xAAAA,0xAAFF,0xAAAA,0xAAAA,
0x4444,0xFF44,0xFF44,0x4444, 0x4444,0x44CC,0x0000,0x0000, 0x0000,0x0077,0x4444,0x4444, 0xEEEE,0xEEEE,0xEEEE,0xEEEE,
0x0000,0x0000,0xEEEE,0xEEEE, 0xCCCC,0xCCCC,0xCCCC,0xCCCC, 0x6666,0x6666,0x6666,0x6666, 0xEEEE,0xEEEE,0x0000,0x0000,
0x0000,0x55AA,0xAA55,0x0000, 0x0044,0xAACC,0xAAAA,0xCC00, 0x00EE,0xAA88,0x8888,0x0000, 0x0000,0x00EE,0xAAAA,0x0000,
0x00EE,0x8844,0x88EE,0x0000, 0x0000,0x77AA,0xAA44,0x0000, 0x0000,0xAAAA,0xEE88,0x0000, 0x0000,0xEE44,0x4422,0x0000,
0xEE44,0xEEAA,0xEE44,0xEE00, 0x0044,0xAAEE,0xAA44,0x0000, 0x00EE,0xAAAA,0xEE44,0xEE00, 0x0066,0x44AA,0xAA44,0x0000,
0x0000,0x6699,0x6600,0x0000, 0x0044,0x44AA,0xAA44,0x4400, 0x0066,0x8844,0x8866,0x0000, 0x0000,0x0044,0xAAAA,0x0000,
0x00EE,0x00EE,0x00EE,0x0000, 0x0044,0xEE44,0x00EE,0x0000, 0x0088,0x4422,0x4488,0xEE00, 0x0022,0x4488,0x4422,0xEE00,
0x0022,0x5544,0x4444,0x4444, 0x2222,0x2222,0x22AA,0x4400, 0x0044,0x00EE,0x0044,0x0000, 0x0066,0xCC00,0x66CC,0x0000,
0x44AA,0x4400,0x0000,0x0000, 0x0000,0x0066,0x6600,0x0000, 0x0000,0x0044,0x0000,0x0000, 0x0033,0x2222,0xAA44,0x0000,
0xCCAA,0xAA00,0x0000,0x0000, 0xCC44,0x88CC,0x0000,0x0000, 0x0000,0xEEEE,0xEE00,0x0000, 0x0000,0x0000,0x0000,0x0000,
};


static void vdpchar64(int pAddr, int ch)
{
    extern int text64_scroll;
    static unsigned int last_offset = 0xffff;
    static unsigned int buf[4];
    unsigned int mask = pAddr & 1 ? 0xf0f0 : 0x0f0f, mask2 = ~mask;
    const unsigned int *font = font3x5 + ch*4;
    unsigned offset = (pAddr - gImage) / 2 ; // byte offset
    offset = (offset & 0xff1f) + ((offset + text64_scroll) & 0x00e0);
    offset = offset * 8 + gPattern;
    if (offset != last_offset) {
        vdpmemread(offset, (unsigned char*) buf, 8);
        last_offset = offset;
    }
    buf[0] = (buf[0] & mask) | (*font++ & mask2);
    buf[1] = (buf[1] & mask) | (*font++ & mask2);
    buf[2] = (buf[2] & mask) | (*font++ & mask2);
    buf[3] = (buf[3] & mask) | (*font++ & mask2);
    vdpmemcpy(offset, (unsigned char*) buf, 8);

    extern unsigned int conio_scrnCol; // conio_bgcolor.c
    vdpmemset(offset - gPattern + gColor, conio_scrnCol, 8);
}

extern unsigned char vdp_bigbuf[256];

void fast_scrn_scroll64() {
    // similar to the slow_scrn_scroll, but uses a larger fixed
    // buffer for far more speed
    const int line = nTextEnd - nTextRow + 1;

    extern unsigned int conio_scrnCol; // conio_bgcolor.c
    extern int text64_scroll;
    unsigned char *buf = (unsigned char *)0x8320;
    int i, src, dst, ch;

    src = (8*32 + text64_scroll) * 8; // source = first middle
    dst = (0*32 + text64_scroll) * 8; // dest = last top

    text64_scroll = (text64_scroll + 0x20) & 0x00e0;
    ch = text64_scroll;

    VDP_SET_ADDRESS_WRITE(gImage);
    for (i = 0; i < 7*32; i++) {
	VDPWD = ch++;
    }

    // copy 32 patterns from middle to top
    vdpmemread(gPattern + src, vdp_bigbuf, 256);
    vdpmemcpy(gPattern + dst, vdp_bigbuf, 256);
    vdpmemread(gColor + src, vdp_bigbuf, 256);
    vdpmemcpy(gColor + dst, vdp_bigbuf, 256);

    VDP_SET_ADDRESS_WRITE(gImage + 7*32);
    for (i = 0; i < 8*32; i++) {
	VDPWD = ch++;
    }

    // copy 32 patterns from bottom to middle
    src += 8*32*8;
    dst += 8*32*8;
    vdpmemread(gPattern + src, vdp_bigbuf, 256);
    vdpmemcpy(gPattern + dst, vdp_bigbuf, 256);
    vdpmemread(gColor + src, vdp_bigbuf, 256);
    vdpmemcpy(gColor + dst, vdp_bigbuf, 256);

    VDP_SET_ADDRESS_WRITE(gImage + 15*32);
    for (i = 0; i < 9*32; i++) {
	VDPWD = ch++;
    }

    // clear the last line
    dst += 8*32*8;
    vdpmemset(gPattern + dst, 0, 256);
    vdpmemset(gColor + dst, conio_scrnCol, 256);


    return;
}
