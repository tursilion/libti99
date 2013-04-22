#define KSCAN_MODE	*((volatile unsigned char*)0x8374)
#define KSCAN_KEY	*((volatile unsigned char*)0x8375)
#define KSCAN_JOYX	*((volatile unsigned char*)0x8376)
#define KSCAN_JOYY	*((volatile unsigned char*)0x8377)
#define KSCAN_STATUS *((volatile unsigned char*)0x837c)
#define KSCAN_MASK	0x20

#define KSCAN_MODE_LAST		0
#define KSCAN_MODE_LEFT		1
#define KSCAN_MODE_RIGHT	2
#define KSCAN_MODE_994		3
#define KSCAN_MODE_PASCAL	4
#define KSCAN_MODE_BASIC	5

#define JOY_LEFT	0xfc
#define JOY_RIGHT	0x04
#define JOY_UP		0x04
#define JOY_DOWN	0xfc

unsigned char kscan(unsigned char mode);
