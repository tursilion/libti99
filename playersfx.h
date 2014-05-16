// Tursi's compressed VGM player -- allows interrupt-based playback
// of custom compressed VGM files (you'll need the compressor tool)
// Player uses a workspace at >8322 (32-bytes), as well as its own
// block of RAM for decompression. 

//*********************
// Use these functions if you want music AND sound effects. If you only
// want music, use player.h which will use less memory and CPU time.
//*********************

//*********************
// Player Data
//*********************

// pointers into the song data - all include the command nibble. Assumes player uses workspace at >8322
extern volatile unsigned int * const pVoice;	// four words, one each for channel pitch (mangled, see below)
extern volatile unsigned char * const pVol;		// four bytes, one each for channel volume
extern volatile unsigned char * const pDone;	// single byte, non-zero if playing, 0x00 when done - is a bitmask of channels still playing

// note, the pitch bytes are reversed, and as written to the sound chip. This means:
// for a tone channel, 0x2381 is a pitch value (on channel 0) of 0x0123
// for noise, 0x??E1 is a noise type 1 (and the ?? byte is not guaranteed)

//*********************
// Player functions
//*********************

// stinitsfx - initialize a song stream playback
// Inputs: pSong: points to the compressed song data (in CPU memory)
//         index: index of the song to play (0 if not a multi-bank song)
void stinitsfx(const void *pSong, const int index);

// ststopsfx - stop playback of the song stream
void ststopsfx();

// sfxinitsfx - initialize a sound effect playback
// Inputs: pSong: points to the compressed song data (in CPU memory)
//         index: index of the sfx to play (0 if not a multi-bank)
//		   priority: priority of this sfx. If less than currently playing,
//					 this one will be ignored, else the other is replaced
//					 sfx always has priority over music
void sfxinitsfx(const void *pSong, const int index, const int priority);

// sfxstopsfx - stop playback of current sfx
void sfxstopsfx();

// allstopsfx - stops music and sfx both
void allstopsfx();

// stplaysfx - play one tick (song and sfx)
// You must call this function once every 60hz interrupt. It is acceptable
// to load it directly into the VDP interrupt hook.
void stplaysfx();

// stcount - returns how many songs are in a pack
// inputs - pSong - pointer to song data
// returns - count (which is just the table pointers subtracted and divided)
unsigned int stcount(const void *pSong);
