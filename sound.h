// Helpers for direct and console interrupt sound processing

//*********************
// direct sound chip access
//*********************
#define SOUND		*((volatile unsigned char*)0x8400)

//*********************
// console sound interrupt pointers
//*********************
// pointer to load the address (in VDP or GROM) of your sound list
#define SOUND_PTR	*((volatile unsigned int*)0x83cc)

// countdown byte - set to '1' to start processing on the next interrupt
// You can also read this back, if it is zero, then the list is complete.
#define SOUND_CNT	*((volatile unsigned char*)0x83ce)

// Flag byte - set SOUND_VDP_MASK to indicate a sound list is in VDP instead of GROM
#define SOUND_VDP	*((volatile unsigned char*)0x83fd)
#define SOUND_VDP_MASK	0x01

//*********************
// console sound interrupt helpers (remember you still have to enable interrupts in your main loop!)
//*********************

// set the address of your sound list
inline void SET_SOUND_PTR(unsigned int x)	{	SOUND_PTR = x;					}

// set that the sound list is in VDP
inline void SET_SOUND_VDP()					{	SOUND_VDP |= SOUND_VDP_MASK;	}

// set that the sound list is in GROM
inline void SET_SOUND_GROM()				{	SOUND_VDP &= ~SOUND_VDP_MASK;	}

// start processing sound list on the next interrupt
inline void START_SOUND()					{	SOUND_CNT = 1;					}
