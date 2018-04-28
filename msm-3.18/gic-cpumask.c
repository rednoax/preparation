#include <stdio.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
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
u32 gic_map[] = {
0x2,0xd8,0x31,0xac,0x3a,0xa6,0x35,0x68,0x3e,0xde,0xff,0x12,0xff,0x13,0xff,0x14,0xff,0x23,0xff,0x27,0xff,0x28,0xff,0x2f,0xff,0x38,0xff,0x39,0xff,0x3a,0xff,0x3b,0xff,0x3c,0xff,0x3d,0xff,0x41,0xff,0x45,0xff,0x49,0xff,0x4a,0xff,0x4b,0xff,0x4c,0xff,0x4e,0xff,0x4f,0xff,0x55,0xff,0x56,0xff,0x5a,0xff,0x5c,0xff,0x5d,0xff,0x61,0xff,0x66,0xff,0x6c,0xff,0x6d,0xff,0x70,0xff,0x72,0xff,0x7e,0xff,0x80,0xff,0x81,0xff,0x83,0xff,0x88,0xff,0x89,0xff,0x8a,0xff,0x8c,0xff,0x8d,0xff,0x8e,0xff,0x8f,0xff,0x90,0xff,0x91,0xff,0x92,0xff,0x93,0xff,0x94,0xff,0x95,0xff,0x96,0xff,0x97,0xff,0x98,0xff,0x99,0xff,0x9b,0xff,0x9d,0xff,0xa7,0xff,0xaa,0xff,0xad,0xff,0xae,0xff,0xaf,0xff,0xb0,0xff,0xb1,0xff,0xb2,0xff,0xb3,0xff,0xb5,0xff,0xbc,0xff,0xbd,0xff,0xbe,0xff,0xbf,0xff,0xc0,0xff,0xc1,0xff,0xc2,0xff,0xc3,0xff,0xc4,0xff,0xc5,0xff,0xc6,0xff,0xc8,0xff,0xc9,0xff,0xca,0xff,0xcb,0xff,0xcc,0xff,0xcd,0xff,0xce,0xff,0xcf,0xff,0xd7,0xff,0xe0,0xff,0xef,0xff,0xf0,0xff,0xfd,0xff,0x104,0xff,0x105,0xff,0x106,0xff,0x107,0xff,0x10d,0xff,0x10e,0xff,0x110,0xff,0x111,0xff,0x112,0xff,0x113,0xff,0x114,0xff,0x115,0xff,0x11d,0xff,0x11e,0xff,0x11f,0xff,0x141,0xff,0x142,0xff,0x143,0xff,0x145,0xff,0x158	
};
u32 gpio_map[] = {
	0x3,0x26,0x4,0x1,0x5,0x5,0x6,0x9,0x8,0x25,0x9,0x24,0xa,0xd,0xb,0x23,0xc,0x11,0xd,0x15,0xe,0x36,0xf,0x22,0x10,0x1f,0x11,0x3a,0x12,0x1c,0x13,0x2a,0x14,0x19,0x15,0xc,0x16,0x2b,0x17,0x2c,0x18,0x2d,0x19,0x2e,0x1a,0x30,0x1b,0x41,0x1c,0x5d,0x1d,0x61,0x1e,0x3f,0x1f,0x46,0x20,0x47,0x21,0x48,0x22,0x51,0x23,0x7e,0x24,0x5a,0x25,0x80,0x26,0x5b,0x27,0x29,0x28,0x7f,0x29,0x56,0x32,0x43,0x33,0x49,0x34,0x4a,0x35,0x3e,0x36,0x7c,0x37,0x3d,0x38,0x82,0x39,0x3b,0x3b,0x32
};

void dump_array(u32 *gic_map, u32 size)
{
	int i, j;
	for (i = 0; i < size; i += 2) {
		int hw = i + 1;
		printf("pin %02d(%02x), hwirq %04d(%04x)", gic_map[i], gic_map[i], gic_map[hw], gic_map[hw]);
		for (j = 1; j < size; j += 2) {
			if (j == hw)
				continue;
			else if (gic_map[j] == gic_map[hw])//if there are 2 pairs with the same hwirq, *** will be appended
				printf("***");
		}
		printf("\n");
	}	
}
struct devres {//sizeof==4 when gcc -m32; ==8 when no -m32
	int i;//if no the following, sizeof(devres) is 4
	unsigned long long data[];
};
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
	dump_array(gic_map, ARRAY_SIZE(gic_map));
	dump_array(gpio_map, ARRAY_SIZE(gpio_map));
	printf("sizeof devres==%d\n", sizeof(struct devres));
	return 0;
}