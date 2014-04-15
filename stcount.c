// stcount function for compressed music player

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
