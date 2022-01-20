// VDP header for the TI-99/4A by Tursi aka Mike Brent
// 4/21/2013
// This code and library released into the Public Domain
// You can copy this file and use it at will ;)

#ifndef VDP_H
#define VDP_H

//*********************
// VDP access ports
//*********************

// Read Data
#define VDPRD	*((volatile unsigned char*)0x8800)
// Read Status
#define VDPST	*((volatile unsigned char*)0x8802)
// Write Address/Register
#define VDPWA	*((volatile unsigned char*)0x8C02)
// Write Data
#define VDPWD	*((volatile unsigned char*)0x8C00)

//*********************
// Inline VDP helpers
//*********************

// the TI only needs a delay between setting the read address and reading,
// and only with certain instruction combinations in fast memory. SO, this
// function does not delay here.
inline void VDP_SAFE_DELAY() {	}

// Set VDP address for read (no bit added)
inline void VDP_SET_ADDRESS(unsigned int x)							{	VDPWA=((x)&0xff); VDPWA=((x)>>8);			}

// Set VDP address for write (adds 0x4000 bit)
inline void VDP_SET_ADDRESS_WRITE(unsigned int x)					{	VDPWA=((x)&0xff); VDPWA=(((x|0x4000)>>8));	}

// Set VDP write-only register 'r' to value 'v'
inline void VDP_SET_REGISTER(unsigned char r, unsigned char v)		{	VDPWA=(v); VDPWA=(0x80|(r));				}

// get a screen offset for 32x24 graphics mode
inline int VDP_SCREEN_POS(unsigned int r, unsigned int c)			{	return (((r)<<5)+(c));						}

// get a screen offset for 40x24 text mode
inline int VDP_SCREEN_TEXT(unsigned int r, unsigned int c)			{	return (((r)<<5)+((r)<<3)+(c));				}

// get a screen offset for 80x24 text mode
inline int VDP_SCREEN_TEXT80(unsigned int r, unsigned int c)	    {	return (((r)<<6)+((r)<<4)+(c));				}

// get a screen offset for 64x24 graphics mode
// NOTE: This is not a real VDP address, it's a virtual address that vdpchar64 understands
// Do not add gImage to it!
inline int VDP_SCREEN_TEXT64(unsigned int r, unsigned int c)			{	return (((r)<<6)+(c));						}

//*********************
// VDP Console interrupt control
//*********************

// Interrupt counter - incremented each interrupt
#define VDP_INT_COUNTER			*((volatile unsigned char*)0x8379)

// Maximum number of sprites performing automatic motion
#define VDP_SPRITE_MOTION_MAX	*((volatile unsigned char*)0x837a)

// Copy of the VDP status byte. If VDP interrupts are enabled, you should read
// this value, instead of reading it directly from the VDP.
#define VDP_STATUS_MIRROR		*((volatile unsigned char*)0x837b)

// This flag byte allows you to turn parts of the console interrupt handler on and off
// See the VDP_INT_CTRL_* defines below
#define VDP_INT_CTRL			*((volatile unsigned char*)0x83c2)

// Address of a user-defined function to call during the vertical interrupt handler,
// set to 0x0000 if not using
#define VDP_INT_HOOK			*((volatile void**)0x83c4)

// If using KSCAN, you must put a copy of VDP register 1 (returned by the 'set' functions)
// at this address, otherwise the first time a key is pressed, the value will be overwritten.
// The console uses this to undo the screen timeout blanking.
#define VDP_REG1_KSCAN_MIRROR	*((volatile unsigned char*)0x83d4)

// The console counts up the screen blank timeout here. You can reset it by writing 0,
// or prevent it from ever triggering by writing an odd number. Each interrupt, it is
// incremented by 2, and when the value reaches 0x0000, the screen will blank by setting
// the blanking bit in VDP register 1. This value is reset on keypress in KSCAN.
#define VDP_SCREEN_TIMEOUT		*((volatile unsigned int*)0x83d6)

// These values are flags for the interrupt control 
	// disable all processing (screen timeout and user interrupt are still processed)
	#define VDP_INT_CTRL_DISABLE_ALL		0x80
	// disable sprite motion
	#define VDP_INT_CTRL_DISABLE_SPRITES	0x40
	// disable sound list processing
	#define VDP_INT_CTRL_DISABLE_SOUND		0x20
	// disable QUIT key testing
	#define VDP_INT_CTRL_DISABLE_QUIT		0x10

// wait for a vblank (interrupts disabled - will work unreliably if enabled)
// call vdpwaitvint() instead if you want to keep running the console interrupt
// DO NOT USE the non-CRU version - this will miss interrupts.
//#define VDP_WAIT_VBLANK  		while (!(VDPST & VDP_ST_INT)) { }
#define VDP_WAIT_VBLANK_CRU	  __asm__( "clr r12\nvdp%=:\n\ttb 2\n\tjeq vdp%=\n\tmovb @>8802,r12" : : : "r12" );
// This version lets you get the status register into a variable (pass the desired variable)
#define VDP_WAIT_VBLANK_CRU_STATUS(x)	  __asm__( "clr r12\n\t\nvdp%=:\n\ttb 2\n\tjeq vdp%=\n\tmovb @>8802,%0" : "=rm" (x) : : "r12" );

// we enable interrupts via the CPU instruction, not the VDP itself, because it's faster
// Note that on the TI interrupts DISABLED is the default state
#define VDP_INT_ENABLE			__asm__("LIMI 2")
#define VDP_INT_DISABLE			__asm__("LIMI 0")
#define VDP_INT_POLL	\
	VDP_INT_ENABLE;		\
	VDP_INT_DISABLE;

//*********************
// Register settings
//*********************

// Bitmasks for the status register
#define VDP_ST_INT				0x80		// interrupt ready
#define VDP_ST_5SP				0x40		// 5 sprites-on-a-line
#define VDP_ST_COINC			0x20		// sprite coincidence
#define VDP_ST_MASK				0x1f		// mask for the 5 bits that indicate the fifth sprite on a line

// these are the actual write-only register indexes
#define VDP_REG_MODE0			0x00		// mode register 0
#define VDP_REG_MODE1			0x01		// mode register 1
#define VDP_REG_SIT				0x02		// screen image table address (this value times 0x0400)
#define VDP_REG_CT				0x03		// color table address (this value times 0x0040)
#define VDP_REG_PDT				0x04		// pattern descriptor table address (this value times 0x0800)
#define VDP_REG_SAL				0x05		// sprite attribute list address (this value times 0x0080)
#define VDP_REG_SDT				0x06		// sprite descriptor table address (this value times 0x0800)
#define VDP_REG_COL				0x07		// screen color (most significant nibble - foreground in text, least significant nibble - background in all modes)

// settings for mode register 0
#define VDP_MODE0_BITMAP		0x02		// set bitmap mode
#define VDP_MODE0_EXTVID		0x01		// enable external video (not connected on TI-99/4A)
#define VDP_MODE0_80COL                 0x04            // enable 9938/F18A 80 column

// settings for mode register 1
#define VDP_MODE1_16K			0x80		// set 16k mode (4k mode if cleared)
#define VDP_MODE1_UNBLANK		0x40		// set to enable display, clear to blank it
#define VDP_MODE1_INT			0x20		// enable VDP interrupts
#define VDP_MODE1_TEXT			0x10		// set text mode
#define VDP_MODE1_MULTI			0x08		// set multicolor mode
#define VDP_MODE1_SPRMODE16x16	0x02		// set 16x16 sprites
#define VDP_MODE1_SPRMAG		0x01		// set magnified sprites (2x2 pixels) 

// sprite modes for the mode set functions
#define VDP_SPR_8x8				0x00
#define	VDP_SPR_8x8MAG			(VDP_MODE1_SPRMAG)
#define VDP_SPR_16x16			(VDP_MODE1_SPRMODE16x16)
#define VDP_SPR_16x16MAG		(VDP_MODE1_SPRMODE16x16 | VDP_MODE1_SPRMAG)

// VDP colors
#define COLOR_TRANS				0x00
#define COLOR_BLACK				0x01
#define COLOR_MEDGREEN			0x02
#define COLOR_LTGREEN			0x03
#define COLOR_DKBLUE			0x04
#define COLOR_LTBLUE			0x05
#define COLOR_DKRED				0x06
#define COLOR_CYAN				0x07
#define COLOR_MEDRED			0x08
#define COLOR_LTRED				0x09
#define COLOR_DKYELLOW			0x0A
#define COLOR_LTYELLOW			0x0B
#define COLOR_DKGREEN			0x0C
#define COLOR_MAGENTA			0x0D
#define COLOR_GRAY				0x0E
#define COLOR_WHITE				0x0F

//*********************
// VDP related functions
//*********************

// set_graphics - sets up graphics I mode - 32x24, 256 chars, color, sprites
// Inputs: pass in VDP_SPR_xxx for the sprite mode you want
// Return: returns a value to be written to VDP_REG_MODE1 (and VDP_REG1_KSCAN_MIRROR if you use kscan())
// The screen is blanked until you do this write, to allow you time to set it up
int set_graphics_raw(int sprite_mode);
// this version enables the screen and sets the KSCAN copy for you
void set_graphics(int sprite_mode);

// set_text - sets up text mode - 40x24, 256 chars, monochrome (color set by VDP_REG_COL), no sprites
// Inputs: none
// Return: returns a value to be written to VDP_REG_MODE1 (and VDP_REG1_KSCAN_MIRROR if you use kscan())
// The screen is blanked until you do this write, to allow you time to set it up
int set_text_raw();
// this version enables the screen and sets the KSCAN copy for you
void set_text();

// set_text80 - sets up 80 column text mode - 80x24. 
// Inputs: none
// Return: returns a value to be written to VDP_REG_MODE1 (and VDP_REG1_KSCAN_MIRROR if you use kscan())
// The screen is blanked until you do this write, to allow you time to set it up
int set_text80_raw();
// this version enables the screen and sets the KSCAN copy for you
void set_text80();

// set_text80_color - sets up 80 column text mode - 80x24 with Position Attributes (F18A only!)
// Inputs: none
// this version enables the screen and sets the KSCAN copy for you
// Use bgcolor and textcolor functions from conio to change colors.
// Return: returns a value to be written to VDP_REG_MODE1 (and VDP_REG1_KSCAN_MIRROR if you use kscan())
// The screen is blanked until you do this write, to allow you time to set it up
int set_text80_color_raw();
// this version enables the screen and sets the KSCAN copy for you
void set_text80_color();

// set_text80_color - sets up 80 column text mode - 80x30 with Position Attributes (F18A only!)
// Inputs: none
// this version enables the screen and sets the KSCAN copy for you
// Use bgcolor and textcolor functions from conio to change colors.
// Return: returns a value to be written to VDP_REG_MODE1 (and VDP_REG1_KSCAN_MIRROR if you use kscan())
// The screen is blanked until you do this write, to allow you time to set it up
int set_text80x30_color_raw();
// this version enables the screen and sets the KSCAN copy for you
void set_text80x30_color();

// set_text64_color - sets up simulated 64-column text mode in bitmap mode - 64x24
// Inputs: none
// this version enables the screen and sets the KSCAN copy for you
// Use bgcolor and textcolor functions from conio to change colors.
void set_text64_color();



// set_multicolor - sets up multicolor mode - 64x48, 256 chars, color, sprites
// Inputs: pass in VDP_SPR_xxx for the sprite mode you want
// Return: returns a value to be written to VDP_REG_MODE1 (and VDP_REG1_KSCAN_MIRROR if you use kscan())
// The screen is blanked until you do this write, to allow you time to set it up
int set_multicolor_raw(int sprite_mode);
// this version enables the screen and sets the KSCAN copy for you
void set_multicolor(int sprite_mode);

// set_bitmap - sets up graphics II (aka bitmap) mode - 32x24, 768 chars in three zones, color, sprites
// Inputs: pass in VDP_SPR_xxx for the sprite mode you want
// Return: returns a value to be written to VDP_REG_MODE1 (and VDP_REG1_KSCAN_MIRROR if you use kscan())
// The screen is blanked until you do this write, to allow you time to set it up
int set_bitmap_raw(int sprite_mode);
// this version enables the screen and sets the KSCAN copy for you
void set_bitmap(int sprite_mode);

// writestring - writes an arbitrary string of characters at any position on the screen
// Inputs: row and column (zero-based), NUL-terminated string to write
// Note: supports text mode
void writestring(int row, int col, char *pStr);

// vdpmemset - sets a count of VDP memory bytes to a value
// Inputs: VDP address to start, the byte to set, and number of repeats
void vdpmemset(int pAddr, int ch, int cnt);

// vdpmemcpy - copies a block of data from CPU to VDP memory
// Inputs: VDP address to write to, CPU address to copy from, number of bytes to copy
void vdpmemcpy(int pAddr, const unsigned char *pSrc, int cnt);

// vdpmemread - copies a block of data from VDP to CPU memory
// Inputs: VDP address to read from, CPU address to write to, number of bytes to copy
void vdpmemread(int pAddr, unsigned char *pDest, int cnt);

// vdpwriteinc - writes an incrementing sequence of values to VDP
// Inputs: VDP address to start, first value to write, number of bytes to write
// This is intended to be useful for setting up bitmap and multicolor mode with
// incrementing tables
void vdpwriteinc(int pAddr, int nStart, int cnt);

// vdpchar - write a character to VDP memory (NOT to be confused with basic's CALL CHAR)
// Inputs: VDP address to write, character to be written
// use indirect function call for each mode
extern void (*vdpchar)(int pAddr, int ch);
void vdpchar_default(int pAddr, int ch);

// vdpreadchar - read a character from VDP memory
// Inputs: VDP address to read
// Outputs: byte
unsigned char vdpreadchar(int pAddr);

// vdpwritescreeninc - like vdpwriteinc, but writes to the screen image table
// Inputs: offset from the screen image table to write, first value to write, number of bytes to write
void vdpwritescreeninc(int pAddr, int nStart, int cnt);

// vdpscreenchar - like vdpchar, but writes to the screen image table
// Inputs: offset from the screen image table to write to, value to be written
void vdpscreenchar(int pAddr, int ch);

// vdpwaitvint - enables console interrupts, then waits for one to happen
// Interrupts are disabled upon exit.
void vdpwaitvint();

// putchar - writes a single character with limited formatting to the bottom of the screen
// Inputs: character to emit
// Returns: character input
// All characters are emitted except \r and \n which is handled for scrn_scroll and next line.
// It works in both 32x24 and 40x24 modes. Tracking of the cursor is thus 
// automatic in this function, and it pulls in scrn_scroll.
int putchar(int x);

// putstring - writes a string with limited formatting to the bottom of the screen
// Inputs: NUL-terminated string to write
// This function only emits printable ASCII characters (32-127). It works in both
// 32x24 and 40x24 modes. It recognizes \r to go to the beginning of the line, and
// \n to go to a new line and scroll the screen. Tracking of the cursor is thus 
// automatic in this function, and it pulls in scrn_scroll.
void putstring(char *s);

// puts - calls putstring, but adds a carriage return. Needed for gcc compatibility
// always returns 1
int puts(char *s);

// printf - writes a string with limited formatting. Only supports a very small subset
// of formatting at the moment. Supports width (for most fields), s, u, i, d, c and X
// (X is byte only). This function will call in putchar().
// Inputs: format string, and varable argument list
// Returns: always returns 0
int printf(char *str, ...);

// hexprint - generates a 2 character hex string from an int and calls putstring to print it
void hexprint(unsigned char x);

// fast_hexprint - generates a 2 character hex string from an int and calls putstring to print it
// uses a 512 byte lookup table - so it is fast but costs more to use
void fast_hexprint(unsigned char x);

// faster_hexprint - works like fast_hexprint but displays directly to VDPWD, no formatting or
// scroll and you must set the VDP address before calling
void faster_hexprint(unsigned char x);

// scrn_scroll - scrolls the screen upwards one line - works in 32x24, 40x24 and 80x24 modes
// the pointer let you replace it, particularly with fast_scrn_scroll
void scrn_scroll_default();
extern void (*scrn_scroll)();

// fast_scrn_scroll- does the same, but uses 256 fixed bytes to do it faster
void fast_scrn_scroll();

// hchar - repeat a character horizontally on the screen, similar to CALL HCHAR
// Inputs: row and column (0-based, not 1-based) to start, character to repeat, number of repetitions (not optional)
// Note: for a single character, vdpscreenchar() is more efficient
void hchar(int r, int c, int ch, int cnt);

// vchar - repeat a character vertically on the screen, similar to CALL VCHAR
// Inputs: row and column (0-based, not 1-based), character to repeat, number of repetitions (not optional)
// Note: for a single character, vdpscreenchar() is more efficient
void vchar(int r, int c, int ch, int cnt);

// gchar - return a character from the screen, similar to CALL GCHAR
// Inputs: row and column (0-based, not 1-based) to read from
// Return: character at the specified position on the screen
unsigned char gchar(int r, int c);

// sprite - set up an entry in the sprite attribute list, similar to CALL SPRITE
// Inputs: sprite number (0-31), character (0-255), color (COLOR_xx), row and column (0-based)
// Note that motion set up is not handled by this function
// Note that row 255 is the first line on the screen
// And finally, note that a row of 208 will disable display of all subsequent sprite numbers
void sprite(int n, int ch, int col, int r, int c);

// delsprite - remove a sprite by placing it offscreen
// Inputs: sprite number (0-31) to hide
void delsprite(int n);

// charset - load the default character set from GROM. This will load both upper and lowercase (small capital) characters.
// Not compatible with the 99/4, if it matters.
void charset();

// charsetlc - load the default character set including true lowercase. This code includes a lower-case character
// set and shifts the GROM character set to align better with it. Because it pulls in data, it does take a little more
// memory (208 bytes). Not compatible with the 99/4, if it matters.
void charsetlc();

// gplvdp - copy data from a GPL function to VDP memory. 
// Inputs: address of a GPL vector, VDP address to copy to, number of characters to copy
// This is a very specialized function used by the charset() functions. It assumes a GPL 'B'
// instruction at the vector, and that the first instruction thereafter is a 'DEST'. It uses
// this to find the actual character set data regardless of the GROM version. This function
// is not compatible with the 99/4 (because it copies 7 bytes per character, and the 99/4
// only provided 6 bytes).
void gplvdp(int vect, int adr, int cnt);

// bm_setforeground - specify foreground color to use when drawing
void bm_setforeground(int c);

// bm_setbackground - specify background color to use when drawing
void bm_setbackground(int c);

// bm_clearscreen - clear the screen and sets all regions to the
//                  current colors
void bm_clearscreen();

// bm_setpixel - turn a pixel on
// Inputs: x - 0-255 - horizontal location
//         y - 0-192 - vertial location
void bm_setpixel(unsigned int x, unsigned int y);

// bm_clearpixel - turn a pixel off
// Inputs: x - 0-255 - horizontal location
//         y - 0-192 - vertial location
void bm_clearpixel(unsigned int x, unsigned int y);

// bm_drawline - plot a line between two points, mode 0 = clear, mode 1 = set
void bm_drawline(int x0, int y0, int x1, int y1, int mode);

// bm_drawlinefast - plot a line between two points
// this version can DRAW (0), ERASE (1) or XOR (2)
// Erase and XOR modes skip the color update
// this function is faster but corrupts scratchpad from >8320 to >833F
// extra setup time may make it slower for short lines
// finally, color table must be at >2000 and pattern table at >0000 (setbitmap does this)
void bm_drawlinefast(int x0, int y0, int x1, int y1, int mode);

// bm_sethlinefast - draws a very fast horizontal line (there's not much speedup possible for vertical)
// does NOT change the color
void bm_sethlinefast(unsigned int x0, unsigned int y0, unsigned int x1);

// bm_clearhlinefast - clears a very fast horizontal line (there's not much speedup possible for vertical)
void bm_clearhlinefast(unsigned int x0, unsigned int y0, unsigned int x1);

// bm_consolefont - loads console font to vdp, then copies it up into ram for
// later use in bitmap mode. Use this before switching to bitmap mode if
// you want a TI font.
void bm_consolefont();

// bm_putc - draw a character at a tile location.
// Inputs : c - character column  0:31
//          r - character row  0:23
void bm_putc(int c, int r, unsigned char alphanum);

// bm_puts - draw a 0 terminated string at a tile location.
//    this provides no scrolling, or bounds limiting.
// Inputs : c - character column  0:31
//          r - character row  0:23
void bm_puts(int c, int r, unsigned char* str);

// bm_placetile - draw a 8x8 pattern at the given tile.
// Inputs : c - character column  0:31
//          r - character row  0:23
void bm_placetile(int c, int r, const unsigned char* pattern);

#define BM_FONT_SIZE (8*(127-32))

// globals holding pen color and background for bitmap drawing
// 0xF0 nibble is foreground, 0x0F nibble is background.
// or use bm_setforeground and bm_setbackground
extern unsigned char gBitmapColor;

// address of bitmap mode font pattern table in cpu memory. 
// This can be intialized by calling bm_consolefont(), or
// by setting it to your own characterset patterns spanning
// characters ' ' to '~'
extern unsigned char* gBmFont;

// global pointers for all to enjoy - make sure the screen setup code updates them!
// assumptions here are for E/A environment, they may not be accurate and your
// program should NOT trust them until after one of the mode set functions is called.
extern unsigned int gImage;				// SIT, Register 2 * 0x400
extern unsigned int gColor;				// CR,  Register 3 * 0x40
extern unsigned int gPattern;				// PDT, Register 4 * 0x800
extern unsigned int gSprite;				// SAL, Register 5 * 0x80
extern unsigned int gSpritePat;			// SDT, Register 6 * 0x800

// text position information used by putstring and scrn_scroll
extern int nTextRow,nTextEnd;
extern int nTextPos,nTextFlags;

// bitflags for nTextFlags - no guessing! ;)
// used for things that the generic code makes decisions on
#define TEXT_FLAG_HAS_ATTRIBUTES 0x8000		// attribute table at gColor
#define TEXT_FLAG_IS_BITMAPPED   0x4000		// graphics in a bitmapped mode
#define TEXT_FLAG_IS_MULTICOLOR	 0x2000		// graphics in multicolor mode
#define TEXT_FLAG_IS_F18A        0x1000		// mode is F18A specific
#define TEXT_WIDTH_32            0x0800     // I wonder if I'll regret bitflags for width...
#define TEXT_WIDTH_40            0x0400
#define TEXT_WIDTH_64            0x0200
#define TEXT_WIDTH_80            0x0100
#define TEXT_HEIGHT_24           0x0080
#define TEXT_HEIGHT_30           0x0040

extern unsigned char gSaveIntCnt;	// console interrupt count byte

// 512 byte lookup table for converting a byte to two ASCII hex characters
extern const unsigned int byte2hex[256];

// f18a specific utility
// enable extended f18a VDP functions
void unlock_f18a();

// lock f18a into 9918 mode
void lock_f18a();

#endif /* VDP_H */
