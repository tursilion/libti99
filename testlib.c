#include "vdp.h"
#include "halt.h"

int main() {
	int x = set_text();
	VDP_SET_REGISTER(VDP_REG_MODE1, x);
	charsetlc();
	putstring("hello world!\n");
	halt();

	return 0;
}


