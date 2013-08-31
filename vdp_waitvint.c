#include "vdp.h"

unsigned char gSaveIntCnt;	// console interrupt count byte

void vdpwaitvint() {
	// wait for a vertical interrupt to occur (enables interrupts - first call may not wait)
	VDP_INT_ENABLE; 
	while (VDP_INT_COUNTER == gSaveIntCnt) { } 
	gSaveIntCnt=VDP_INT_COUNTER; 
	VDP_INT_DISABLE; 
}
