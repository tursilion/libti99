// String code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

// signed int
char *int2str(int x) {
  static char strbuf[8];
  
  // we just populate and return strbuf
  int unzero = 0;
  char *out = strbuf;
  int div = 10000;
  int tmp;
  
  // biggest value is -32767, so just run with it
  if (x < 0) {
    *(out++)='-';
    x=0-x;    // make positive
  }
  
  while (div > 0) {
    tmp = x/div;
    x = x % div;
    if ((tmp > 0) || (unzero)) {
      unzero = 1;
      *(out++) = tmp+'0';
    }
    div /= 10;
  }
  if (!unzero) *(out++)='0';
  *out = '\0';
  return strbuf;
}
