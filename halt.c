#include "vdp.h"
#include "halt.h"

void halt() {
	// wait for the user to press QUIT
	VDP_INT_ENABLE;
	for (;;) { }
}
