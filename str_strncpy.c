// String code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

char *strncpy(char *d, const char *s, int len) {
  char *ret = d;
  while ((*s)&&(len--)) *(d++)=*(s++);
  *d='\0';
  return ret;
}
