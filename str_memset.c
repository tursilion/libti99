// String code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

void *memset(void *dest, int src, int cnt) {
  char *d = (char*)dest;
  while (cnt > 0) {
    *(d++) = src;
    --cnt;
  }
  return dest;
}
