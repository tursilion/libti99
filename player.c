//*********************
// Use these functions if you want ONLY music. If you want sound effects
// AND music together, use sfxplayer, which will need a little more
// memory and CPU but is otherwise the same.
//*********************

// pointers into the song data - all include the command nibble. Assumes player uses workspace at >8322
volatile unsigned int * const pVoice = (volatile unsigned int*)0x833A;
volatile unsigned char * const pVol = (volatile unsigned char*)0x8334;
volatile unsigned char * const pDone = (volatile unsigned char*)0x8330;

// stcount - returns how many songs are in a pack
// inputs - pSong - pointer to song data
// returns - count (which is just the table pointers subtracted and divided)
unsigned int stcount(const void *pSong) {
	unsigned int ret;

	// using inline assembly because I don't want gcc to
	// pull in division just for this simple piece of math
	__asm__(
		"mov *%1+,r2\n\t"
		"mov *%1,r3\n\t"
		"s r2,r3\n\t"
		"clr r2\n\t"
		"li %1,24\n\t"
		"div %1,r2\n\t"
		"mov r2,%0\n\t"
		 : "=r"(ret) : "r"(pSong) : "r2", "r3"		// todo: unfortunate to force these, how do we specify 2 regs for div?
	);

	return ret;
}
