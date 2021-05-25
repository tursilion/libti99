#include <vdp.h>
#include <kscan.h>

// read from keyboard - limits to maxlen chars
// no autorepeat :)
void gets(char *buf, int maxlen) {
  char oldch;
  int cnt = maxlen;
  
  oldch = 255;
  
  while (cnt) {
    vdpchar(nTextPos, 30);  // cursor
    while (kscan(5) == oldch) {  // wait for key, 99/4 mode, allow interrupts
      VDP_INT_ENABLE;
      VDP_INT_DISABLE;
    }
    oldch = KSCAN_KEY;
    switch (oldch) {
      case '\r':
        cnt = 0;
        *buf = '\0';
        vdpchar(nTextPos, ' ');
        putchar('\n');
        // eat all extra repeating ENTER values.
        // this happens if the ENTER key is held down for instance
        while (kscan(5) == oldch) {
          VDP_INT_ENABLE;
          VDP_INT_DISABLE;
        }
        break;
      
      case 8:   // backspace
        if (cnt < maxlen) {
          ++cnt;
          vdpchar(nTextPos, ' ');
          --nTextPos;
          --buf;
        }
        break;
      
      default:
        if ((oldch >= ' ') && (oldch <= 'Z') && (cnt > 1)) {
          putchar(oldch);
          *(buf++)=oldch;
          --cnt;
        }
        break;
    }
  }
}
