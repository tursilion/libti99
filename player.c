// pointers into the song data - all include the command nibble. Assumes player uses workspace at >8322
volatile unsigned int * const pVoice = (volatile unsigned int*)0x833A;
volatile unsigned char * const pVol = (volatile unsigned char*)0x8334;
volatile unsigned int * const pDone = (volatile unsigned int*)0x8330;


