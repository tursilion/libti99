// pointers into the song data - all include the command nibble. Assumes player uses workspace at >8320
volatile unsigned int * const pVoice = (unsigned int*)0x8338;
volatile unsigned char * const pVol = (unsigned char*)0x8332;
volatile unsigned int * const pDone = (unsigned int*)0x832E;


