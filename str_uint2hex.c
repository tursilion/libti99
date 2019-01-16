// unsigned int to hex string
char* uint2hex(unsigned int x) {
  static char strbuf[5];

  // we just populate and return strbuf
  char *out = strbuf;
  unsigned int div = 0x1000;
  int digit = 0;
  
  while (div > 0) {
    digit = x/div;
    x = x % div;
    if (digit > 9) {
      *(out++) = digit - 10 + 'A';
    } else {
      *(out++) = digit + '0';
    }
    div /= 0x10;
  }
  *out = '\0';
  return strbuf;
}