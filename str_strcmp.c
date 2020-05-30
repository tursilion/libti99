// String code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)

int strcmp(const char *s1, const char *s2) {
	while (*s1) {
		if (*s2 == '\0') {
			return 1;
		}
		if (*s1 != *s2) {
			return *s1 - *s2;
		}
		++s1;
		++s2;
	}
	if (*s2 != '\0') {
		return -1;
	}
	return 0;
}
