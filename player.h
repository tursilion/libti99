// should be a sound ref somewhere
#define SOUND *((volatile unsigned char*)0x8400)

// pointers into the song data - all include the command nibble. Assumes player uses workspace at >8320
extern volatile unsigned int * const pVoice;	// four words, one each for channel pitch (mangled, see below)
extern volatile unsigned char * const pVol;		// four bytes, one each for channel volume
extern volatile unsigned int * const pDone;		// single word, 0xffff if playing, 0x0000 when done

// note, the pitch bytes are reversed, and as written to the sound chip. This means:
// for a tone channel, 0x2381 is a pitch value (on channel 0) of 0x0123
// for noise, 0x??E1 is a noise type 1 (and the ?? byte is not guaranteed)

void stinit(const void *pSong);
void ststop();
void stplay();
