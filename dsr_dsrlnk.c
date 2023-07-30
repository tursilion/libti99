// DSR interface code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#include "files.h"
#include "vdp.h"
#include "string.h"

// NOTE: because this does not return the entire PAB back to you,
// if you need data from the DSR other than the error byte
// (ie: RECORD NUMBER), then you have to get it yourself!
unsigned char dsrlnk(struct PAB *pab, unsigned int vdp) {
	unsigned char x;

	// copies your PAB to VDP and then executes the call through dsrlnkraw
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

	// now we can call it
	if (dsrlnkraw(vdp)) {
        unsigned char ret = vdpreadchar(vdp+1);
        if (ret == 0) {
            return DSR_ERR_DSRNOTFOUND;
        } else {
            return GET_ERROR(ret);
        }
    }

	// all good, presumably
	return DSR_ERR_NONE;
}

