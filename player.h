// Tursi's compressed VGM player -- allows interrupt-based playback
// of custom compressed VGM files (you'll need the compressor tool)
// Player uses a workspace at >8322 (32-bytes), as well as its own
// block of RAM for decompression. 

//*********************
// Use these functions if you want ONLY music. If you want sound effects
// AND music together, use sfxplayer, which will need a little more
// memory and CPU but is otherwise the same.
//*********************

#ifndef PLAYER_H
#define PLAYER_H

//*********************
// Player Data
//*********************

// pointers into the song data - all include the command nibble. Assumes player uses workspace at >8322
extern volatile unsigned int * const pVoice;	// four words, one each for channel pitch (mangled, see below)
extern volatile unsigned char * const pVol;		// four bytes, one each for channel volume
extern volatile unsigned char * const pDone;	// single word, 0xff if playing, 0x00 when done

// note, the pitch bytes are reversed, and as written to the sound chip. This means:
// for a tone channel, 0x2381 is a pitch value (on channel 0) of 0x0123
// for noise, 0x??E1 is a noise type 1 (and the ?? byte is not guaranteed)

//*********************
// Player functions
//*********************

// stinit - initialize a stream playback
// Inputs: pSong: points to the compressed song data (in CPU memory)
//         index: index of the song to play (0 if not a multi-bank song)
void stinit(const void *pSong, const int index);

// ststop - stop playback
void ststop();

// stplay - play one tick
// You must call this function once every 60hz interrupt. It is acceptable
// to load it directly into the VDP interrupt hook.
void stplay();

// stcount - returns how many songs are in a pack
// inputs - pSong - pointer to song data
// returns - count (which is just the table pointers subtracted and divided)
unsigned int stcount(const void *pSong);

#endif /* PLAYER_H */
