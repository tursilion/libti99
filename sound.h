// direct sound chip access
#define SOUND		*((volatile unsigned char*)0x8400)

// console sound interrupt pointers
#define SOUND_PTR	*((volatile unsigned int*)0x83cc)
#define SOUND_CNT	*((volatile unsigned char*)0x83ce)
#define SOUND_VDP	*((volatile unsigned char*)0x83fd)
#define SOUND_VDP_MASK	0x01

// console sound interrupt helpers (remember you still have to enable interrupts in your main loop!)
inline void SET_SOUND_PTR(unsigned int x)	{	SOUND_PTR = x;					}
inline void SET_SOUND_VDP()					{	SOUND_VDP |= SOUND_VDP_MASK;	}
inline void SET_SOUND_GROM()				{	SOUND_VDP &= ~SOUND_VDP_MASK;	}
inline void START_SOUND()					{	SOUND_CNT = 1;					}
