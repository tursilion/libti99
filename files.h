// DSR interface code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)


// *** WARNING *** This code does NOT work on hardware yet.

#define DSR_FILES_COUNT	*((volatile unsigned char*)0x834C)
#define DSR_LEN_COUNT	*((volatile unsigned int*)0x8354)
#define DSR_PAB_POINTER *((volatile unsigned int*)0x8356)

#define DSR_OPEN	0x00
#define DSR_CLOSE	0x01
#define DSR_READ	0x02
#define DSR_WRITE	0x03
#define DSR_REWIND	0x04
#define DSR_LOAD	0x05
#define DSR_SAVE	0x06
#define DSR_DELETE	0x07
#define DSR_SCRATCH	0x08	// note: most DSRs do not implement scratch
#define DSR_STATUS	0x09

#define GET_ERROR(status) ((status)&0xe0)
	#define DSR_ERR_NONE			0x00
	#define DSR_ERR_WRITEPROTECT	0x20
	#define DSR_ERR_BADATTRIBUTE	0x40
	#define DSR_ERR_ILLEGALOPCODE	0x60
	#define DSR_ERR_MEMORYFULL		0x80
	#define DSR_ERR_PASTEOF			0xA0
	#define DSR_ERR_DEVICEERROR		0xC0
	#define DSR_ERR_FILEERROR		0xE0

// DSR_TYPE not intended for comparisons, since there are lots of 0x00 ;)
#define DSR_TYPE_VARIABLE	0x10
#define DSR_TYPE_FIXED		0x00

#define DSR_TYPE_INTERNAL	0x08
#define DSR_TYPE_DISPLAY	0x00

#define DSR_TYPE_UPDATE		0x00
#define DSR_TYPE_OUTPUT		0x02
#define DSR_TYPE_INPUT		0x04
#define DSR_TYPE_APPEND		0x06

#define DSR_TYPE_RELATIVE	0x01
#define DSR_TYPE_SEQUENTIAL	0x00

// these are valid masks for testing after a status call - some devices return after every call
// but this may not be relied upon.
#define DSR_STATUS_NOTFOUND		0x80
#define DSR_STATUS_PROTECTED	0x40
#define DSR_STATUS_INTERNAL		0x10		// else type is display
#define DSR_STATUS_PROGRAM		0x08		// else type is non-program
#define DSR_STATUS_VARIABLE		0x04		// else record length is fixed
#define DSR_STATUS_MEMORYFULL	0x02		// else device has space remaining
#define DST_STATUS_EOF			0x01		// else not at end of file

// PAB struct
struct __attribute__((__packed__)) PAB {
	unsigned char OpCode;			// see DSR_xxx list above
	unsigned char Status;			// file type and error code (DSR_ERR_xxx and DSR_TYPE_xxx)
	unsigned int  VDPBuffer;		// address of the data buffer in VDP memory
	unsigned char RecordLength;		// size of records. Not used for PROGRAM type. >00 on open means autodetect
	unsigned char CharCount;		// number of bytes read or number of bytes to write
	unsigned int  RecordNumber;		// record number for normal files, available bytes (LOAD or SAVE) for PROGRAM type
	unsigned char ScreenOffset;		// Used in BASIC for screen BIAS. Also returns file status on Status call. (DSR_STATUS_xxx)
	unsigned char NameLength;		// for this implementation only, set to zero to read the length from the string
	unsigned char *pName;			// for this implementation only, must be a valid C String even if length is set
};


// Set maximum number of open files
// Inputs: number of files (1-9 valid on most devices, consumes VRAM from the top of memory)
void files(unsigned char count);

// Perform a DSR function per the passed in PAB, which is installed in VRAM at the specified address
// Inputs: pab - pointer to the populated PAB struct
//         vdp - address in VDP to store the PAB (10 bytes plus length of filename)
// Returns: 0 on success, or PAB error on failure
unsigned char dsrlnk(struct PAB *pab, unsigned int vdp);

// Execute a DSR link on the PAB already in VDP. Use this if you know the VDP PAB is already
// updated and you don't want the overhead of copying it again.
// Inputs: pointer to PAB in VDP
// Note: no return code. Read it from the PAB yourself. :)
void dsrlnkraw(unsigned int vdppab);
