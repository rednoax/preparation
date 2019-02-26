#include <stdio.h>
//TTBCR test
int main()
{
	int i;
	unsigned long addr;
	for (i = 1; i <= 7; i++) {
		addr = (1UL<<32) - (1UL<<(32 - i));
		printf("%d: %lx\t%08lx\n", i, addr, 1UL<<(32 - i));
	}
	return 0;
}
