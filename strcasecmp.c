#include <stdio.h>
#include <ctype.h>//for int tolower(int), int toupper(int)
/*	s1		s2		return
"a"			"ab"	<0(0-'b')
"ab"		"a"		>0('b')
"abc"		"ABC"	0
*/
int strcasecmp(const char *s1, const char *s2)
{
	char c1;
	char c2;
#if 0//v1: cannot work for "abc" vs "ABC"
	for (c1 = *s1, c2 = *s2; c1 && c2 && c1 == c2; c1 = *s1, c2 = *s2) {
		s1++;
		s2++;
	}
#endif
#if 0//v2: too verbose
	for (c1 = tolower(*s1), c2 = tolower(*s2); c1 && c2 && c1 == c2; c1 = tolower(*s1), c2 = tolower(*s2)) {
		s1++;
		s2++;
	}
#endif
	do {
		c1 = tolower(*s1++);
		c2 = tolower(*s2++);
	} while (c1 == c2 && c1);
	return c1 - c2;
}

int main(int argc, char **argv)
{
	int ret;
	const char *s1 = argv[1];
	const char *s2 = argv[2];

	if (argc < 3) {
		printf("usage: add 2 string\n");
		return 0;
	}
	ret = strcasecmp(s1, s2);
	printf("\"%s\" - \"%s\": %d\n", s1, s2, ret);
	return 0;
}
