// Sample app using conio to print hello.

#include <conio.h>
#include <vdp.h>

const char newDef[8] = { 0x3c,0x42,0xa5,0x81,0xa5,0x99,0x42,0x3c };

int main(void)
{
    set_graphics(VDP_SPR_16x16);                        // set video mode
    charsetlc();                                        // load character set with lowercase
    vdpmemset(gImage, 32, 768);                         // clear screen
    VDP_SET_REGISTER(VDP_REG_COL, COLOR_MEDGREEN);   	// set screen color
    vdpmemset(gColor, 0x10, 32);                        // set colors to black on transparent
   
    vdpmemcpy(gPattern+42*8, newDef, 8);                // redefine character 42

    gotoxy(2,2);										// set cursor position

    cprintf("Hello *");									// say hi

    while(1) {											// spin forever
        VDP_INT_POLL;                                   // enable interrupts during the loop so QUIT works
    };

    return 0;
}
