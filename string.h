// String code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

#ifndef STRING_H
#define STRING_H

// strlen - returns the length of a zero terminated string
int strlen(const char *s);

// converts a string to an integer. No overflow detection, ignores
// leading whitespace, and converts to the first non-digit character.
int atoi(char *s);

// copy a zero-terminated string from s to d, returns d
char *strcpy(char *d, const char *s);

// compare two zero-terminated strings, return >0 if s1 is greater,
// <0 if s2 is greater, or 0 if the strings are equal
int strcmp(const char *s1, const char *s2);

// compare two memory blocks, return >0 if s1 is greater,
// <0 if s2 is greater, or 0 if the blocks are equal
int memcmp(const void *s1, const void *s2, int n);

// copy a block of memory from src to dest, of cnt bytes. Returns dest.
void *memcpy(void *dest, const void *src, int cnt);

// set a block of memory at dest to src, of cnt bytes. Returns dest.
void *memset(void *dest, int src, int cnt);

// returns a pointer to a static string, a number converted as unsigned
// Not thread safe, don't use from interrupt handlers.
char *uint2str(unsigned int x);

// returns a pointer to a static string, a number converted as signed
// Not thread safe, don't use from interrupt handlers.
char *int2str(int x);

// returns a pointer to a static hex string, a number converted as unsigned
// Not thread safe, don't use from interrupt handlers.
char* uint2hex(unsigned int x);

// reads a string from the keyboard - pulls in KSCAN. Uses keyboard mode 5.
// Displays a solid cursor. The only edit key supported is Fctn-S. Stops
// at maxlen, returned string is zero-terminated. Needs KSCAN, VDP, etc.
void gets(char *buf, int maxlen);

#endif /* STRING_H */
