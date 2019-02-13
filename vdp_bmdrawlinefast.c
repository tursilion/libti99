
#include "vdp.h"

void bm_drawlinefast(int x1, int y1, int x2, int y2, int mode) {
	__asm__(
	"	mov %0,@>8324		; y2 -> r2\n"
	"	mov %1,@>8326		; x2 -> r3\n"
	"	mov %2,@>8328		; y1 -> r4\n"
	"	mov %3,@>832a		; x1 -> r5\n"
	"	mov %4,@>833a		; mode -> r13\n"
	"	movb @gBitmapColor,@>833c	; color -> r14 MSB\n"
	"	bl @bm_asmdrawlinefast\n"
	:
	: "r" (y2), "r" (x2), "r" (y1), "r" (x1), "r" (mode)
	: "r11"
	);
}
