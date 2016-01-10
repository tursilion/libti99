// String code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

// string to integer
int atoi(char *s) {
  int old;
  int out = 0;
  int neg = 0;
  
  // skip whitespace
  while ((*s <= ' ') && (*s != '\0')) ++s;
  
  if (*s == '-') {
    ++s;
    neg = 1;
  }
  if (*s == '+') ++s;

  // check whole digits
  while ((*s >= '0') && (*s <= '9')) {
    old = out;
    out *= 10;
    out += (*s)-'0';
    ++s;
    if (out < old) {
      // too big, wrapped around
      return 0;
    }
  }

  if (neg) out = 0-out;
  return out;
}

