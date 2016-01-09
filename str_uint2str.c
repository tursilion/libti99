// String code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

// unsigned int
char *uint2str(unsigned int x) {
  static char strbuf[8];

  // we just populate and return strbuf
  int unzero = 0;
  char *out = strbuf;
  int div = 10000;
  int tmp;
  
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
