// GROM code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

// default GROM access port addresses (base 0)
#define GROMRD_0			(0x9800)
#define GROMRA_0			(0x9802)
#define GROMWD_0			(0x9C00)
#define GROMWA_0			(0x9c02)

// Default (base 0) GROM access ports
#define GROMRD			*((volatile unsigned char*)GROMRD_0)
#define GROMRA			*((volatile unsigned char*)GROMRA_0)
#define GROMWD			*((volatile unsigned char*)GROMWD_0)
#define GROMWA			*((volatile unsigned char*)GROMWA_0)

inline void GROM_SET_ADDRESS(unsigned int x)		{	GROMWA=((x)>>8); GROMWA=((x)&0xff);							}
inline unsigned int GROM_GET_ADDRESS()				{	unsigned int ret; ret=(GROMRA)<<8; ret|=GROMRA; return ret; }
