#include "conio.h"

void chline(int v) {
    vdpmemset(conio_getvram(), '-', v);
}
