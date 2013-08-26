// DSR interface code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "files.h"

unsigned int __attribute__((noinline)) searchdofiles(unsigned int base) {
	unsigned int ret;

	*(unsigned int*)0x83f8 = base;		// GPLWS R12

	// TODO: we could rewrite the rest of this in C, just adding support for SBO, SBZ and the actual call which
	// needs to be wrapped with LWPI....
	__asm__(
	"seto %0			; not found\n\t"
	"lwpi 0x83e0		; gplws\n\t"
	"sbo 0				; turn on the rom (set above)\n\t"
	"mov @0x400a,r1		; get pointer to the subprogram list\n"
"filelp\n\t"
	"jeq filedon		; no subprograms\n\t"
	"mov *r1+,r3		; link to next item\n\t"
	"mov *r1+,r2		; address of this one\n\t"
	"mov *r1+,r0		; we are looking for length 1, name >16\n\t"
	"ci r0,0x0116\n\t"
	"jeq filegt\n\t"
	"mov r3,r1			; nope, get next\n\t"
	"jmp filelp\n"
"filegt\n\t"
	"lwpi 0x8300\n\t"
	"clr %0				; mark success in gcc workspace\n\t"
	"lwpi 0x83e0\n\t	; we aren't done yet\n\t"
	"bl *r2				; go ahead and call it\n\t"
	"nop				; skipped on success (we ignore failure, then)\n"
"filedon\n\t"
	"sbz 0				; turn off the rom (we assume r12 was not altered, it shouldn't be!)\n\t"
	"lwpi 0x8300		; our own ws back\n\t"
	: "=r" (ret)
	);

	return ret;
}

void files(unsigned char count) {
	DSR_FILES_COUNT = count;
	unsigned int dsrbase = 0x1000;
	while (searchdofiles(dsrbase)) {
		dsrbase+=0x100;
		if (dsrbase >= 0x2000) break;
	}
}
