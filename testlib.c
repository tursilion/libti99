#include "system.h"
#include "puff.h"
#include "conio.h"
#include "math.h"
#include "kscan.h"
#include "speech.h"

unsigned char helloworldraw[] = {
  0xF3,0x48,0xCD,0xC9 ,0xC9 ,0x57 ,0x70 ,0xF6 ,0xF7 
  ,0x0D ,0x08 ,0x72 ,0x0D ,0x0E ,0x76 ,0x75 ,0x51 
  ,0x28 ,0xCF ,0x2F ,0xCA ,0x49 ,0x51 ,0xE4 ,0x02, 0x00
};

// MUST be large enough for output data - you need to know!
unsigned char outbuf[32];
unsigned int dstlen;
unsigned int srclen;

// repeat this to test the screen modes
void testprintf() {
    unsigned char x, y;

    gotoxy(0,0);
    cprintf("\fTesting printf...\n\n");
    cprintf("1\t2\t3\t4\t\n");
    cprintf("Superbug\b\b\bman\n");
    cprintf("Bad  Thing\rGood\n");
    cprintf("Broken\v line\n");

    // https://alvinalexander.com/programming/printf-format-cheat-sheet
    cprintf("==3d==\n");
    cprintf("  0 = %3d\n", 0);
    cprintf("12345 = %3d\n", 12345);
    cprintf("-10 = %3d\n", -10);
    cprintf("-12345 = %3d\n", -12345);

    cprintf("==-3d==\n");
    cprintf("  0 = %-3d\n", 0);
    cprintf("12345 = %-3d\n", 12345);
    cprintf("-10 = %-3d\n", -10);
    cprintf("-12345 = %-3d\n", -12345);

    cprintf("==03d==\n");
    cprintf("  0 = %03d\n", 0);
    cprintf("12345 = %03d\n", 12345);
    cprintf("-10 = %03d\n", -10);
    cprintf("-12345 = %03d\n", -12345);

    cgetc();

    cprintf("==+==\n");
    cprintf("%+3d\n", 5);
    cprintf("%+03d\n", 5);
    cprintf("%-+03d\n", 5);
    cprintf("%3.3d\n", 12345);

    cprintf("==Hello==\n");
    cprintf("'%s'\n", "Hello");
    cprintf("'%10s'\n", "Hello");
    cprintf("'%-10s'\n", "Hello");
    cprintf("'%3.3s'\n", "Hello");

    cprintf("Char (Hi!): %c%c%c\n", 'H', 'i', '!');
    cprintf("Decimal (123 -456): %i %d\n", 123, -456);
    cprintf("Unsigned (123 65080): %u %u\n", 123, -456);
    cprintf("Uppercase Hex (ABCD): %X\n", 0xABCD);
    cprintf("Lowercase Hex (abcd): %x\n", 0xABCD);
    cprintf("Octal (17001): %o\n", 7681);
    cprintf("Percent: %%\n");

    screensize(&x, &y);
    cprintf("Screen is %dx%d, Cursor at %d,%d\n", x, y, wherex(), wherey());

    for(int i=0;i<9;i++) {
      cprintf("0123456789");
    }
    cprintf("\n");

    cputs("Press any key press any key Press any key press any key Press any key press any key...");
    cgetc();
}

void testColorText() {
    cprintf("a splash of ");
    bgcolor(COLOR_DKRED);
    textcolor(COLOR_LTBLUE);
    cprintf("color!");
    textcolor(COLOR_WHITE);
    bgcolor(COLOR_DKBLUE);
    cprintf("\npress any key\n");
    cgetc();
}

void testBitmapMode() {
    bm_consolefont();
    set_bitmap(0);

    bm_setbackground(COLOR_CYAN);
    bm_setforeground(COLOR_BLACK);
    bm_clearscreen();

    int r = 0;
    for(int c=0; c<23; c+=1) {
        bm_puts(c, r++, "Hello Bitmaps!");
        bm_setforeground((r % 14)+2);
    }
    bm_setforeground(0);

    bm_puts(0, 23, "Press any key...");
    while(!kbhit());
    cgetc();

    bm_clearscreen();

    bm_puts(0,0, "Pixel test...");

    // set every pixel, each row a new color    
    for(int y=20; y<40; y++) {
        for(int x=0; x<256; x++) {
            bm_setpixel(x, y);
        }
        bm_setforeground(y%16);
    }
    // clear every pixel, one at a time
    bm_setforeground(COLOR_BLACK);
    for(int y=20; y<40; y++) {
        for(int x=0; x<256; x++) {
            bm_clearpixel(x, y);
        }
    }

    bm_puts(0, 23, "Press any key...");
    while(!kbhit());
    cgetc();
    
    // draw a web
    bm_setbackground(COLOR_BLACK);
    bm_setforeground(COLOR_GRAY);
    bm_clearscreen();
    bm_puts(0,0, "Drawline test...");
    int x = 0; 
    int y = 0;
    while(y < 192) {
      bm_drawline(0, y, x, 191,1);
        y += 6;
        x += 6;
    }
	y = 191;
	x = 255;
	while (y >= 0) {
		bm_drawline(255, y, x, 0, 1);
		y -= 6;
		x -= 6;
	}
    bm_puts(0, 23, "Press any key...");
    while(!kbhit());
    cgetc();

    bm_clearscreen();
    bm_puts(0,0, "Fastline test Draw...");
    x = 0; 
    y = 0;
    while(y < 192) {
        bm_drawlinefast(0, y, x, 191, 0);
        y += 6;
        x += 6;
    }
	y = 191;
	x = 255;
	while (y >= 0) {
		bm_drawlinefast(255, y, x, 0, 0);
		y -= 6;
		x -= 6;
	}
    bm_puts(0, 23, "Press any key...");
    while(!kbhit());
    cgetc();

    bm_puts(0,0, "Fastline test Erase...");
    x = 0; 
    y = 0;
    while(y < 192) {
        bm_drawlinefast(0, y, x, 191, 1);
        y += 6;
        x += 6;
    }
	y = 191;
	x = 255;
	while (y >= 0) {
		bm_drawlinefast(255, y, x, 0, 1);
		y -= 6;
		x -= 6;
	}
    bm_puts(0, 23, "Press any key...");
    while(!kbhit());
    cgetc();

    bm_clearscreen();
    bm_puts(0,0, "Fastline test XOR...");
    x = 0; 
    y = 0;
    while(y < 192) {
        bm_drawlinefast(0, y, x, 191, 2);
        y += 6;
        x += 6;
    }
	y = 191;
	x = 255;
	while (y >= 0) {
		bm_drawlinefast(255, y, x, 0, 2);
		y -= 6;
		x -= 6;
	}
    bm_puts(0, 23, "Press any key...");
    while(!kbhit());
    cgetc();

    bm_clearscreen();
    bm_puts(0,0, "Fasthline set......");
    x=5;
	for (y=8; y<192; ++y) {
		bm_sethlinefast(127-x, y, 128+x);
		if (x<127) ++x;
	}
    bm_puts(0, 23, "Press any key...");
    while(!kbhit());
    cgetc();

    bm_clearscreen();
    bm_puts(0,0, "Fasthline clear......");
    for (y=8; y<192; ++y){ 
		bm_sethlinefast(10,y,240);
	}
    x=5;
    int dir=1;
	for (y=8; y<192; ++y) {
		bm_clearhlinefast(127-x, y, 128+x);
		x+=dir;
		if (x == 127) dir=-1;
	}

    bm_puts(0, 23, "Press any key...");
    while(!kbhit());
    cgetc();
    set_text();
}

void test_speech() {
    if (detect_speech()) {
        printf("Speech Synthesizer detected\n");
        say_vocab(VOCAB_GOOD_WORK);
	speech_wait();
    }
}


int main() {
	int f18 = 0;
	
	set_text();
	charsetlc();
    textcolor(COLOR_WHITE);
    bgcolor(COLOR_DKBLUE);
	puts("Include F18A tests? (Y/N)");
	for(;;) {
		kscan(KSCAN_MODE_BASIC);
		if ((KSCAN_KEY == 'Y')||(KSCAN_KEY=='N')) break;
	}
	if (KSCAN_KEY == 'Y') f18=1;
	

    testBitmapMode();

	set_text();
	clrscr();
	charsetlc();
    textcolor(COLOR_WHITE);
    bgcolor(COLOR_DKBLUE);
    putstring("hello world!\n");

    if(abs(75)==75 && abs(-32)==32) {
        putstring("abs function passed.\n");
    }

	putstring("\nsizeof(int):  ");
	hexprint(sizeof(int));
	putstring("\nsizeof(long): ");
	hexprint(sizeof(long));
	putstring("\nsizeof(short):");
	hexprint(sizeof(short));
    putstring("\nsizeof(float):");
    hexprint(sizeof(float));
    putstring("\n");

	dstlen=32;
	srclen=sizeof(helloworldraw);
	int x = puff(outbuf, &dstlen, helloworldraw, &srclen);
	putstring("\npuff() returned: ");
	hexprint(x);
	putstring("\n");
    outbuf[31] = '\0';
	putstring(outbuf);
	putstring("\n");

    // testing conio... better screen output code
    cursor(1);
    gotoxy(0, 0);
    cgetc();

    clrscr();
    cursor(0);

    for (int idx = 1; idx < 5; ++idx) {
        bgcolor(idx);
        bordercolor(idx + 1);
        cgetc();
    }

    cursor(1);
    vdpmemset(gImage, '*', 768);
    gotoxy(2, 5);
    cclear(5);
    cclearxy(2, 15, 10);

    while (!kbhit());
    cgetc();

    clrscr();
    cprintf("press keys: ");
    for (int idx = 0; idx < 10; idx++) {
        unsigned char x = cgetc();
        cputc(x);
    }

    clrscr();
    for (int idx = 0; idx < 5; ++idx) {
        gotoxy(idx, idx);
        chline(5);
        cvline(5);
    }
    for (int idx = 5; idx < 10; ++idx) {
        chlinexy(idx, idx, 10);
        cvlinexy(idx, idx, 10);
    }

    cursor(0);
    cgetc();
    cursor(1);

    testprintf();
    
    set_graphics(0);
    vdpmemset(gColor, COLOR_CYAN<<4, 32);    // set color table
    testprintf();

	if (f18) {
		set_text80();
		charsetlc();       // different VRAM layout, reload charset
		testprintf();
	}

    set_text64_color(); // uses charset from RAM instead of vdp.
    testprintf();
    testColorText();

	if (f18) {
		set_text80_color();
		charsetlc();       // different VRAM layout, reload charset
		testprintf();
		testColorText();
		lock_f18a();
		set_graphics(0);   // reset various other vdp state.

		set_text80x30_color();
		charsetlc();       // different VRAM layout, reload charset
		testprintf();
		testColorText();
		lock_f18a();
		set_graphics(0);   // reset various other vdp state.
	}

    set_text();
    charsetlc();       // different VRAM layout, reload charset
    clrscr();

    for (int idx = 0; idx < 12; ++idx) {
        cputcxy(idx, idx, '*');
        cputcxy(38-idx, idx, '*');
        cputcxy(idx, 23-idx, '*');
        cputcxy(38-idx, 23-idx, '*');   // avoid last screen position or we'll scroll
    }
    cgetc();

    clrscr();
    for (int idx = 0; idx < 5; ++idx) {
        cputsxy(idx * 2, idx, "Hello!");
    }
    cgetc();

    for (int idx = 11; idx < 16; ++idx) {
        textcolor(idx);
        while (!kbhit());
        while (kbhit()) cgetc();
    }
    cgetc();

    test_speech();
    
    printf("** DONE **\n");
	halt();

	return 0;
}


