#include <stdio.h>
#include <ctype.h>
/*	s1		s2		return
"a"			"ab"	<0(0-'b')
"ab"		"a"		>0('b')
*/
int strcasecmp(const char *s1, const char *s2)
{
	char c1;
	char c2;
	for (c1 = *s1, c2 = *s2; c1 && c2 && c1 == c2; c1 = *s1, c2 = *s2) {		
		s1++;
		s2++;
	}
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
