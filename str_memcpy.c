// String code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

void *memcpy(void *dest, const void *src, int cnt) {
  char *d = (char*)dest;
  char *s = (char*)src;
  while (cnt > 0) {
    *(d++) = *(s++);
    --cnt;
  }
  return dest;
}
