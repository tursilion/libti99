// String code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

char *strcpy(char *d, const char *s) {
  char *ret = d;
  while (*s) *(d++)=*(s++);
  *d='\0';
  return ret;
}
