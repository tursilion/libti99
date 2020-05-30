// String code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

int memcmp(const void *s1, const void *s2, int n) {
	unsigned char *p1 = (unsigned char*)s1;
	unsigned char *p2 = (unsigned char*)s2;
	
	while (n-- > 0) {
		if (*p1 != *p2) {
			return *p1 - *p2;
		}
		++p1;
		++p2;
	}
	return 0;
}
