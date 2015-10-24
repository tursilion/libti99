// DSR interface code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "files.h"
#include "vdp.h"

// needs the whole GPL space - check for safe areas in the E/A manual

void __attribute__((noinline)) gpldsrlnk(unsigned int vdp) {
	unsigned char x;

	// copies your PAB to VDP
	vdpmemcpy(vdp, (const unsigned char*)pab, 9);
	// assumes vdpmemcpy leaves the VDP address in the right place!
	if (pab->NameLength == 0) {
		x = strlen(pab->pName);
	} else {
		x= pab->NameLength;
	}
	VDPWD = x;

	// and the filename itself - note we assume 'x' is valid!
	unsigned char *p = pab->pName;
	while (x--) {
		VDPWD = *(p++);
	}

	// set up and call the GPL DSRLNK
	vdp+=9;
	DSR_PAB_POINTER = vdp;

	__asm__(
	"		ai r10,-38				; make stack room to save workspace, rtn & zero word\n"
	"		lwpi 0x83e0				; get gplws\n"
	"		li r0,0x8300			; source wp for backup\n"
	"		mov @0x8314,r1			; get r10 for destination\n"
	"bkupl1	mov *r0+,*r1+			; copy register\n"
	"		ci r0,>8322				; test for end of copy\n"
	"		jne bkupl1\n"
	"		mov @0x3ff6,*r1+		; save the return information\n"
	"		mov @0x3ff8,*r1+		; save the return information\n"
	"		li r1,0x0460			; B instruction\n"
	"		mov r1,@0x3ff6\n"
	"		li r1,dsrret			; return address\n"
	"		mov r1,@0x3ff8\n"
	
	// now need to find the >0f,>4B string which looks like XML >FB, and will jump to >3FF6 (where we planted a B@DSRRET)
	// We have to search, but it should be near >36B8. Make that the return address. Then we load the jump code for
	// >0010 and parse it as per http://nouspikel.group.shef.ac.uk/ti99/gplcall.htm



}
