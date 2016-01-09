// String code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

// string to integer
int atoi(char *s) {
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
    out *= 10;
    out += (*s)-'0';
    ++s;
  }

  if (neg) out = 0-out;
  return out;
}

