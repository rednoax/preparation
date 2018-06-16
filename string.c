#include <stdio.h>
#include <string.h>

void __strcpy(char *dst, const char *src)
{
	const char *dst_org = dst;
	const char *src_org = src;
	while ((*dst++ = *src++))
		;
	printf("%ld %ld\n", dst - dst_org, src - src_org);
}

int main()
{
	char src[16], dst[16];
	strcpy(src, "");
	__strcpy(dst, src);
	strcpy(src, "string");
	__strcpy(dst, src);
	return 0;
}