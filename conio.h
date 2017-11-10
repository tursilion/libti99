// quick and dirty implementation of functions from cc65's conio.h
// note that datatypes may be slightly different, ints are more reliable
// This code and library released into the Public Domain
// You can copy this file and use it at will ;)
// this code assumes 40 columns but much of it will probably work in 32 or 80.
// this code maintains its own cursor position and is not compatible with the 
// vdp_put functions directly.

#include "vdp.h"
#include <stdarg.h>

// enable this define and rebuild the library to free up 256 bytes of buffer space
//#define USE_SLOW_SCROLL

// get a VRAM address based on the screen mode we're in
extern int conio_x,conio_y;
unsigned int conio_getvram();

// bgcolor - sets the screen background color.
extern unsigned int conio_scrnCol;
unsigned int bgcolor(unsigned int color);

// bordercolor - this is the same as the background color on the TI, so we don't set it
unsigned int bordercolor(unsigned int x);

// cclear - write spaces
void cclear(unsigned int length);

// cclearxy - write spaces at a location
void cclearxy(int col, int row, int v);

// read a character from the keyboard
extern int conio_cursorFlag;
extern int conio_cursorChar;
unsigned char cgetc();

// output a horizonal line
void chline(int v);

// output a line at a position
void chlinexy(int xx, int yy, int v);

// clear the screen
void clrscr();

// print formatted text
int cprintf(const char *fmt, ...);

// print a char (with control characters)
void cputc(int ch);

// print a char at location
void cputcxy(int xx, int yy, int ch);

// print a string (knows control codes)
void cputs(const char *s);

// print a string (knows control codes)
void cputsxy(int xx, int yy, const char *s);

// enable/disable the cgetc cursor
#define cursor(x) conio_cursorFlag=(x)

// draw a vertical line
void cvline(int len);

// draw a vertical line at location
void cvlinexy(int x, int y, int len);

// change the x location only
#define gotox(x) conio_x = (x)

// change the xy location
#define gotoxy(x,y) conio_x = (x); conio_y = (y)

// change the y location only
#define gotoy(y) conio_y = (y)

// check for a key - simulates a 1-key buffer
unsigned char kbhit();

// reverses the text display - not supported but you could make your own character set
unsigned char reverse(unsigned char x);

// returns the dimensions of the screen -- this is not valid until you have called set_graphics(), set_text() or set_text80() (80 for F18/9938 only)
void screensize(unsigned char *x, unsigned char *y);

// changes the color of the text - works in TEXT mode only and changes ALL text on the screen
unsigned int textcolor(unsigned int color);

// printf with a format list
int vcprintf(const char *fmt, va_list argp);

// return the x position
inline int wherex() { return conio_x; }

// return the y position
inline int wherey() { return conio_y; }
