// DSR interface code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "files.h"
#include "vdp.h"

// uses: scratchpad >8340-8348, >8354, >8355, >8356, >83d0, >83d2, GPLWS

#define DSR_NAME_LEN	*((volatile unsigned char*)0x8355)

void __attribute__((noinline)) dsrlnkraw(unsigned int vdp) {
	// modified version of the e/a DSRLNK, for data >8 (DSR) only
	// this one does not modify data in low memory expansion so "boot tracking" there may not work.
	unsigned char *buf = (unsigned char*)0x8340;	// 8 bytes of memory for a name buffer
	unsigned int status = vdp + 1;

	vdp+=9;
	DSR_PAB_POINTER = vdp;

	unsigned char size = vdpreadchar(vdp);
	unsigned char cnt=0;
	while (cnt < 8) {
		buf[cnt] = VDPRD;	// still in the right place after the readchar above got the length
		if (buf[cnt] == '.') {
			break;
		}
		cnt++;
	}
	if ((cnt == 0) || (cnt > 7)) {
		// illegal device name length
		vdpchar(status, DSR_ERR_FILEERROR);
		return;
	}
	// save off the device name length (asm below uses it!)
	DSR_LEN_COUNT=cnt;

	unsigned int CRU = 0;
	DSR_NAME_LEN = cnt;
	++cnt;
	DSR_PAB_POINTER += cnt;

	// TODO: we could rewrite the rest of this in C, just adding support for SBO, SBZ and the actual call which
	// needs to be wrapped with LWPI....
	__asm__(
"		jmp begin\n"
"dsrdat data >aa00\n"
"begin  lwpi 0x83e0                ;  	load gplws to call the dsr with\n"
"       clr  r1					;	r1=0\n"
"       li   r12,0x0f00			;	cru base to >0f00 (first card -1)\n"
"       jmp  a2316              ; skip card off.\n"
"a2310  sbz  0              ;        	card off\n"
"a2316  ai   r12,0x0100		;		next card (>1000 for first)\n"
"       clr  @0x83d0	;				clear cru tracking at >83d0\n"
"       ci   r12,0x2000	;			check if all cards are done\n"
"       jeq  a2388       ;           	if yes, we didn't find it, so error out\n"
"       mov  r12,@0x83d0   ;           	save cru base\n"
"       sbo  0             ;         	card on\n"
"       li   r2,0x4000		;			read card header bytes\n"
"       cb   *r2,@dsrdat      ;       	>aa = header\n"
"       jne  a2310            ;      	no: loop back for next card\n"
"       ai   r2,8            	;	offset (contains the data statement, so 8 for a device, for >4008)\n"
"       jmp  a2340				;	always jump into the loop from here\n"
"a233a  mov  @0x83d2,r2           ;    	next sub\n"
"       sbo  0                    ;  	card on (already is, isn't it??)\n"
"a2340  mov  *r2,r2                  ; 	grab link pointer to next\n"
"       jeq  a2310                  ;	if no pointer, link back to get next card\n"
"       mov  r2,@0x83d2               ;	save link address in >83d2\n"
"       inct r2					;	point to entry address\n"
"       mov  *r2+,r9               ;   	save address in r9\n"
"       movb @0x8355,r5			;	get dsr name length (low byte of >8354)\n"
"       jeq  a2364              ;    	size=0, so take it \n"
"       cb   r5,*r2+				;	compare length to length in dsr\n"
"       jne  a233a              ;    	diff size: loop back for next\n"
"       srl  r5,8			;			make length a word count\n"
"       li   r6,%0        ;       	name buffer pointer in r6\n"
"a235c  cb   *r6+,*r2+          ;      	check name\n"
"       jne  a233a             ;     	diff name: loop back for next entry\n"
"       dec  r5					;	count down length\n"
"       jne  a235c              ;    	not done yet: next char\n"
"a2364  inc  r1                  ;    	if we get here, everything matched, increment # calls\n"
"       bl   *r9                 ;    	link\n"
"       jmp  a233a              ;    	check next entry on the same card -- most dsrs will skip this \n"
"       sbz  0                  ;    	card off\n"
"a2388  lwpi 0x8300             ;      exit code\n"
	:
	: "i" (buf)
);

}
