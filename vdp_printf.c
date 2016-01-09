#include "vdp.h"
#include "string.h"
#include <stdarg.h>

/* a quick hacky printf workaround */
/* not written to be efficient, only to be fast to write */
/* always returns 0, and is nowhere near a full implementation */
int printf(char *str, ...) {
  char *p;
  char *s;
  char *orig;
  int v;
  unsigned int u;
  char ch;
  int w,x;
  va_list ap;
  va_start(ap, str);
  
  p = str;
  orig = p;
  while (*p) {
    w = 0;
    if (*p != '%') {
      putchar(*(p++));
    } else {
      ++p;
      while ((*p >= '0') && (*p <= '9')) {
        // width field
        w *= 10;
        w += (*p) - '0';
        ++p;
      }
        
      switch (*p) {
        case '.':
        case 'f':
          // float and/or precision
          // no
          putchar('#');
          ++p;
          break;
        
        case 's':
          // string
          s = va_arg(ap, char*);
          if (w > 0) {
            x=strlen(s);
            w-=x;
            while (w-- > 0) putchar(' ');
          }
          while (*s) putchar(*(s++));
          ++p;
          break;
          
        case 'u':
          // unsigned int
          u = va_arg(ap, unsigned int);
          s = uint2str(u);
          if (w > 0) {
            x = strlen(s);
            w-=x;
            while (w-- > 0) putchar(' ');
          }
          while (*s) putchar(*(s++));
          ++p;
          break;
          
        case 'i':
        case 'd':
          // signed int
          u = va_arg(ap, int);
          s = int2str(u);
          if (w > 0) {
            x = strlen(s);
            w-=x;
            while (w-- > 0) putchar(' ');
          }
          while (*s) putchar(*(s++));
          ++p;
          break;
         
        case 'c':
          // char
          ch = va_arg(ap, int); // char promotes to int for varargs
          putchar(ch);
          ++p;
          break;
          
        case 'X':
          // uppercase hex - only expects a char for debug here anyway
          // no precision or double bytes...
          u = va_arg(ap, int);
          hexprint(u&0xff);
          ++p;
          break;
          
        default:
          putchar(*p);
          ++p;
          break;
      }
    } // else
  } // while
  
  va_end(ap);
  return 0;
} // printf

