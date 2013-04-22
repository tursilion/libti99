// Tursi's compressed VGM player -- allows interrupt-based playback
// of custom compressed VGM files (you'll need the compressor tool)
// Player uses a workspace at >8322 (32-bytes), as well as its own
// block of RAM for decompression. 

//*********************
// This code is still subject to change.
//*********************

//*********************
// Player Data
//*********************

// pointers into the song data - all include the command nibble. Assumes player uses workspace at >8322
extern volatile unsigned int * const pVoice;	// four words, one each for channel pitch (mangled, see below)
extern volatile unsigned char * const pVol;		// four bytes, one each for channel volume
extern volatile unsigned int * const pDone;		// single word, 0xffff if playing, 0x0000 when done

// note, the pitch bytes are reversed, and as written to the sound chip. This means:
// for a tone channel, 0x2381 is a pitch value (on channel 0) of 0x0123
// for noise, 0x??E1 is a noise type 1 (and the ?? byte is not guaranteed)

//*********************
// Player functions
//*********************

// stinit - initialize a stream playback
// Inputs: points to the compressed song data (in CPU memory)
void stinit(const void *pSong);

// ststop - stop playback
void ststop();

// stplay - play one tick
// You must call this function once every 60hz interrupt. It is acceptable
// to load it directly into the VDP interrupt hook.
void stplay();
