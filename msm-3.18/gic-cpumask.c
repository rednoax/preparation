#include <stdio.h>
typedef unsigned char u8;
typedef unsigned int u32;
static u8 gic_get_cpumask()//struct gic_chip_data *gic)
{
	u32 mask = 0x04030201;//readl_relaxed(base + GIC_DIST_TARGET + i);
	/*
	mask	a	b	c	d
	>>16	0	0	a	b
	|=		a	b	a|c	b|d
	>>8		0	a	b	a|c
	|=		a	a|b		a|b|c		a|b|c|d
	so at last a|b|c|d is returned
	*/
	mask |= mask >> 16;
	mask |= mask >> 8;
	return mask;
}

int main()
{
	int i;
	for (i = 0; i < 32; i += 4) {
		printf("%d\n", i);
	}
	for (i = 32; i < 32 * (10 + 1); i += 4) {
		if (i != ((i * 4) / 4))
			printf("***");
		printf("%d:%d\n", i, i * 4 / 4);
	}
	u32 cpumask = gic_get_cpumask();
	printf("%08x\n", cpumask);
	cpumask |= cpumask << 8;
	cpumask |= cpumask << 16;
	printf("%08x\n", cpumask);
	return 0;
}