#include <stdio.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

struct exception_table_entry
{
	unsigned long insn, fixup;
};

struct exception_table_entry *__start___ex_table, *__stop___ex_table;

static int cmp_ex(const void *a, const void *b)
{
	const struct exception_table_entry *x = a, *y = b;
	if (x->insn > y->insn)
		return 1;
	if (x->insn < y->insn)
		return -1;
	return 0;
}

typedef unsigned int u32;

void u32_swap(void *a, void *b, int size)
{
	u32 t = *(u32 *)a;
	*(u32 *)a = *(u32 *)b;
	*(u32 *)b = t;
}

static void generic_swap(void *a, void *b, int size)
{
	char t;
	do {
		t = *(char *)a;
		*(char *)a++ = *(char *)b;
		*(char *)b++ = t;
	} while (--size > 0);
}

void sort(void *base, size_t num, size_t size,
	int (*cmp_func)(const void *, const void *),
	void (*swap_func)(void *, void *, int size))
{
	int i = (num/2 - 1) * size, n = num * size, c, r;

	if (!swap_func)
		swap_func = (size == 4 ? u32_swap : generic_swap);

	for ( ; i >= 0; i -= size) {
		for (r = i; r * 2 + size < n; r = c) {
			c = r * 2 + size;
			if (c < n - size && cmp_func(base + c, base + c + size) < 0)
				c += size;
			if (cmp_func(base + r, base + c) >= 0)
				break;
			swap_func(base + r, base + c, size);
		}
	}

	for (i = n - size; i > 0; i -= size) {
		swap_func(base, base + i, size);
		for (r = 0; r * 2 + size < i; r = c) {
			c = r * 2 + size;
			if (c < i - size && cmp_func(base + c, base + c + size) < 0)
				c += size;
			if (cmp_func(base + r, base + c) >= 0)
				break;
			swap_func(base + r, base + c, size);
		}
	}
}

unsigned long array[] = {
	/*
	dumpped via:
	(gdb) p __start___ex_table
	$9 = 0xc02b6000
	(gdb) p __stop___ex_table-__start___ex_table
	$10 = 464
	(gdb) p 464*8
	$11 = 3712
	(gdb) p 464*2
	$12 = 928
	(gdb) x/928 __start___ex_table
	*/
	0xc0022bc4,0xc022c6f0,0xc00252d0,0xc022c6f4,
	0xc00252e8,0xc022c6fc,0xc0025364,0xc022c704,
	0xc0025370,0xc022c70c,0xc002537c,0xc022c714,
	0xc0025388,0xc022c71c,0xc0025394,0xc022c724,
	0xc00253a0,0xc022c72c,0xc00253ac,0xc022c734,
	0xc00253b8,0xc022c73c,0xc00253c4,0xc022c744,
	0xc00253d0,0xc022c74c,0xc00253dc,0xc022c754,
	0xc00253e8,0xc022c75c,0xc00253f4,0xc022c764,
	0xc0025400,0xc022c76c,0xc002540c,0xc022c774,
	0xc0025418,0xc022c77c,0xc0025424,0xc022c784,
	0xc0025440,0xc022c78c,0xc002544c,0xc022c794,
	0xc0025458,0xc022c79c,0xc0025464,0xc022c7a4,
	0xc0025480,0xc022c7ac,0xc002552c,0xc022c7b4,
	0xc0025538,0xc022c7c0,0xc0025544,0xc022c7cc,
	0xc0025550,0xc022c7d8,0xc002555c,0xc022c7e4,
	0xc0025568,0xc022c7f0,0xc0025574,0xc022c7fc,
	0xc0025580,0xc022c808,0xc002558c,0xc022c814,
	0xc0025598,0xc022c820,0xc00255a4,0xc022c82c,
	0xc00255b0,0xc022c838,0xc00255bc,0xc022c844,
	0xc00255c8,0xc022c850,0xc00255d4,0xc022c85c,
	0xc00255e0,0xc022c868,0xc00255ec,0xc022c874,
	0xc00258cc,0xc022c880,0xc00258d8,0xc022c888,
	0xc00259ec,0xc022c890,0xc0025d78,0xc022c898,
	0xc0025d8c,0xc022c8a4,0xc0025da4,0xc022c8b0,
	0xc0025db4,0xc022c8bc,0xc0025e14,0xc022c8c8,
	0xc0025e28,0xc022c8d0,0xc0025e40,0xc022c8d8,
	0xc0025e50,0xc022c8e0,0xc0026a00,0xc022c8e8,
	0xc0026ad4,0xc022c8f4,0xc0026adc,0xc022c900,
	0xc0026af8,0xc022c90c,0xc0027d90,0xc022c918,
	0xc0027d9c,0xc022c920,0xc0027da0,0xc022c920,
	0xc0027e24,0xc022c928,0xc0027e2c,0xc022c934,
	0xc0027e3c,0xc022c940,0xc0027e44,0xc022c94c,
	0xc0027e54,0xc022c958,0xc0027e5c,0xc022c964,
	0xc002aa94,0xc022c970,0xc002aa98,0xc022c978,
	0xc002aaa4,0xc022c980,0xc002aaac,0xc022c980,
	0xc002aac0,0xc022c988,0xc002aac4,0xc022c990,
	0xc002aaf0,0xc022c998,0xc002aaf8,0xc022c998,
	0xc002ab6c,0xc022c9a0,0xc002ab70,0xc022c9a8,
	0xc002ab74,0xc022c9b0,0xc002ab78,0xc022c9b8,
	0xc002aba8,0xc022c9c0,0xc002abac,0xc022c9c8,
	0xc002abb0,0xc022c9d0,0xc002abb4,0xc022c9d8,
	0xc002abc8,0xc022c9e0,0xc002abd0,0xc022c9e0,
	0xc002abd8,0xc022c9e0,0xc002abe0,0xc022c9e0,
	0xc002abf4,0xc022c9e8,0xc002abfc,0xc022c9e8,
	0xc002ac04,0xc022c9e8,0xc002ac0c,0xc022c9e8,
	0xc002ac24,0xc022c9f0,0xc002ac28,0xc022c9f8,
	0xc002ac2c,0xc022ca00,0xc002ac30,0xc022ca08,
	0xc002ac58,0xc022ca10,0xc002ac5c,0xc022ca18,
	0xc002ac60,0xc022ca20,0xc002ac64,0xc022ca28,
	0xc002ac98,0xc022ca30,0xc002aca0,0xc022ca30,
	0xc002aca8,0xc022ca30,0xc002acb0,0xc022ca30,
	0xc002acc4,0xc022ca38,0xc002accc,0xc022ca38,
	0xc002acd4,0xc022ca38,0xc002acdc,0xc022ca38,
	0xc002ad48,0xc022ca40,0xc002ad4c,0xc022ca48,
	0xc002ad50,0xc022ca50,0xc002ad54,0xc022ca58,
	0xc002ad60,0xc022ca60,0xc002ad68,0xc022ca60,
	0xc002ad70,0xc022ca60,0xc002ad78,0xc022ca60,
	0xc002ad8c,0xc022ca68,0xc002ad90,0xc022ca70,
	0xc002ad94,0xc022ca78,0xc002ad98,0xc022ca80,
	0xc002adc4,0xc022ca88,0xc002adcc,0xc022ca88,
	0xc002add4,0xc022ca88,0xc002addc,0xc022ca88,
	0xc002ae58,0xc022ca90,0xc002ae60,0xc022ca9c,
	0xc002b00c,0xc022caa8,0xc002b42c,0xc022cab4,
	0xc002b430,0xc022cabc,0xc002b434,0xc022cac4,
	0xc002b444,0xc022cacc,0xc002b468,0xc022cad4,
	0xc002b470,0xc022cad4,0xc002b478,0xc022cad4,
	0xc002b480,0xc022cad4,0xc002b4e8,0xc022cadc,
	0xc002b4ec,0xc022cae4,0xc002b4f0,0xc022caec,
	0xc002b500,0xc022caf4,0xc002b524,0xc022cafc,
	0xc002b52c,0xc022cafc,0xc002b534,0xc022cafc,
	0xc002b53c,0xc022cafc,0xc0032218,0xc022cb04,
	0xc003eb54,0xc022cb08,0xc003ecf0,0xc022cb10,
	0xc003ed74,0xc022cb18,0xc003ed84,0xc022cb24,
	0xc004bb84,0xc022cb2c,0xc004bb94,0xc022cb34,
	0xc004bbac,0xc022cb3c,0xc004bbf0,0xc022cb44,
	0xc004bbf8,0xc022cb4c,0xc004bc2c,0xc022cb54,
	0xc004bc34,0xc022cb5c,0xc004bc4c,0xc022cb64,
	0xc004bc60,0xc022cb6c,0xc004bc70,0xc022cb74,
	0xc004bc84,0xc022cb7c,0xc004bc90,0xc022cb84,
	0xc004bca4,0xc022cb8c,0xc004bcb8,0xc022cb94,
	0xc004bcc0,0xc022cb9c,0xc004bcd0,0xc022cba4,
	0xc004bce4,0xc022cbac,0xc004bcec,0xc022cbb4,
	0xc004bd04,0xc022cbbc,0xc004bd1c,0xc022cbc4,
	0xc004bd24,0xc022cbcc,0xc004c488,0xc022cbd4,
	0xc004c498,0xc022cbe0,0xc004c4a8,0xc022cbec,
	0xc005cd68,0xc022cbf8,0xc005cd70,0xc022cbf8,
	0xc005e7c4,0xc022cc00,0xc005e7cc,0xc022cc00,
	0xc005eba0,0xc022cc08,0xc005eba8,0xc022cc08,
	0xc005ebb8,0xc022cc10,0xc005ebc0,0xc022cc10,
	0xc005ebd0,0xc022cc18,0xc005ebd8,0xc022cc18,
	0xc005ebe8,0xc022cc20,0xc005ebf0,0xc022cc20,
	0xc005ec00,0xc022cc28,0xc005ec08,0xc022cc28,
	0xc0068838,0xc022cc30,0xc0068868,0xc022cc3c,
	0xc006b100,0xc022cc48,0xc006b15c,0xc022cc50,
	0xc0096b1c,0xc022cc58,0xc0096b4c,0xc022cc64,
	0xc0097110,0xc022cc70,0xc0097138,0xc022cc78,
	0xc009d864,0xc022cc80,0xc009d878,0xc022cc88,
	0xc009d890,0xc022cc90,0xc009d89c,0xc022cc98,
	0xc009d8f0,0xc022cca0,0xc009d90c,0xc022cca8,
	0xc009d990,0xc022ccb0,0xc009d994,0xc022ccb0,
	0xc009d9b0,0xc022ccb8,0xc009d9b4,0xc022ccb8,
	0xc009d9d0,0xc022ccc0,0xc009d9d4,0xc022ccc0,
	0xc009d9ec,0xc022ccc8,0xc009d9f8,0xc022ccd0,
	0xc009da0c,0xc022ccd8,0xc009da64,0xc022cce0,
	0xc009db28,0xc022cce8,0xc009db3c,0xc022ccf0,
	0xc009db58,0xc022ccf8,0xc009db64,0xc022cd00,
	0xc009db8c,0xc022cd08,0xc009dd40,0xc022cd10,
	0xc009dd44,0xc022cd10,0xc009ed40,0xc022cd18,
	0xc009ed4c,0xc022cd20,0xc00a7a68,0xc022cd28,
	0xc00adb98,0xc022cd34,0xc00adbc0,0xc022cd3c,
	0xc00bc2c0,0xc022cd44,0xc00bc2d4,0xc022cd4c,
	0xc00bc2d8,0xc022cd4c,0xc00bc97c,0xc022cd54,
	0xc00bc98c,0xc022cd5c,0xc00bc99c,0xc022cd64,
	0xc00bc9e4,0xc022cd6c,0xc00bc9ec,0xc022cd74,
	0xc00bca20,0xc022cd7c,0xc00bca28,0xc022cd84,
	0xc00bca40,0xc022cd8c,0xc00bca58,0xc022cd94,
	0xc00bca5c,0xc022cd94,0xc00bca70,0xc022cd9c,
	0xc00bca74,0xc022cd9c,0xc00bca88,0xc022cda4,
	0xc00bca98,0xc022cdac,0xc00bcab0,0xc022cdb4,
	0xc00bcab4,0xc022cdb4,0xc00bcac8,0xc022cdbc,
	0xc00bcad4,0xc022cdc4,0xc00bcaf0,0xc022cdcc,
	0xc00bcaf4,0xc022cdcc,0xc00bcb10,0xc022cdd4,
	0xc00bcb18,0xc022cddc,0xc00bcb30,0xc022cde4,
	0xc00bcb34,0xc022cde4,0xc00bcb4c,0xc022cdec,
	0xc00bcb58,0xc022cdf4,0xc00bcb7c,0xc022cdfc,
	0xc00bcb80,0xc022cdfc,0xc00bcb8c,0xc022ce04,
	0xc00bf984,0xc022ce0c,0xc00c4808,0xc022ce18,
	0xc00c4844,0xc022ce20,0xc00c48ac,0xc022ce28,
	0xc00c48ec,0xc022ce30,0xc00c4978,0xc022ce38,
	0xc01363e8,0xc013645c,0xc01363ec,0xc013645c,
	0xc01363f8,0xc013645c,0xc0136434,0xc013645c,
	0xc0136558,0xc022ce40,0xc013655c,0xc022ce40,
	0xc0136560,0xc022ce40,0xc0136570,0xc022ce40,
	0xc0136574,0xc022ce40,0xc0136580,0xc022ce40,
	0xc0136588,0xc022ce40,0xc013658c,0xc022ce40,
	0xc0136594,0xc022ce40,0xc0136618,0xc022ce4c,
	0xc013661c,0xc022ce4c,0xc0136620,0xc022ce4c,
	0xc0136624,0xc022ce4c,0xc0136628,0xc022ce4c,
	0xc013662c,0xc022ce4c,0xc0136630,0xc022ce4c,
	0xc0136634,0xc022ce4c,0xc0136660,0xc022ce4c,
	0xc0136664,0xc022ce4c,0xc0136668,0xc022ce4c,
	0xc013666c,0xc022ce4c,0xc0136670,0xc022ce4c,
	0xc0136674,0xc022ce4c,0xc0136678,0xc022ce4c,
	0xc01366ac,0xc022ce50,0xc01366b0,0xc022ce50,
	0xc01366b4,0xc022ce50,0xc01366d4,0xc022ce50,
	0xc01366d8,0xc022ce50,0xc01366dc,0xc022ce50,
	0xc0136704,0xc022ce50,0xc0136738,0xc022ce44,
	0xc013673c,0xc022ce44,0xc0136740,0xc022ce44,
	0xc0136744,0xc022ce44,0xc0136750,0xc022ce44,
	0xc0136754,0xc022ce44,0xc0136758,0xc022ce44,
	0xc013675c,0xc022ce44,0xc01367bc,0xc022ce50,
	0xc0136800,0xc022ce44,0xc0136804,0xc022ce44,
	0xc0136808,0xc022ce44,0xc013680c,0xc022ce44,
	0xc0136818,0xc022ce44,0xc013681c,0xc022ce44,
	0xc0136820,0xc022ce44,0xc0136824,0xc022ce44,
	0xc0136884,0xc022ce50,0xc01368c8,0xc022ce44,
	0xc01368cc,0xc022ce44,0xc01368d0,0xc022ce44,
	0xc01368d4,0xc022ce44,0xc01368e0,0xc022ce44,
	0xc01368e4,0xc022ce44,0xc01368e8,0xc022ce44,
	0xc01368ec,0xc022ce44,0xc013694c,0xc022ce50,
	0xc0136a34,0xc022ce74,0xc0136a38,0xc022ce74,
	0xc0136a3c,0xc022ce74,0xc0136a40,0xc022ce74,
	0xc0136a44,0xc022ce74,0xc0136a48,0xc022ce74,
	0xc0136a4c,0xc022ce74,0xc0136a50,0xc022ce74,
	0xc0136a9c,0xc022ce74,0xc0136aa0,0xc022ce74,
	0xc0136aa4,0xc022ce74,0xc0136aa8,0xc022ce74,
	0xc0136aac,0xc022ce74,0xc0136ab0,0xc022ce74,
	0xc0136ab4,0xc022ce74,0xc0136acc,0xc022ce78,
	0xc0136ad0,0xc022ce78,0xc0136ad4,0xc022ce78,
	0xc0136af4,0xc022ce78,0xc0136af8,0xc022ce78,
	0xc0136b00,0xc022ce78,0xc0136b98,0xc022ce6c,
	0xc0136b9c,0xc022ce6c,0xc0136ba0,0xc022ce6c,
	0xc0136ba4,0xc022ce6c,0xc0136ba8,0xc022ce6c,
	0xc0136bac,0xc022ce6c,0xc0136bb0,0xc022ce6c,
	0xc0136bb4,0xc022ce6c,0xc0136be0,0xc022ce78,
	0xc0136c64,0xc022ce6c,0xc0136c68,0xc022ce6c,
	0xc0136c6c,0xc022ce6c,0xc0136c70,0xc022ce6c,
	0xc0136c74,0xc022ce6c,0xc0136c78,0xc022ce6c,
	0xc0136c7c,0xc022ce6c,0xc0136c80,0xc022ce6c,
	0xc0136cac,0xc022ce78,0xc0136d30,0xc022ce6c,
	0xc0136d34,0xc022ce6c,0xc0136d38,0xc022ce6c,
	0xc0136d3c,0xc022ce6c,0xc0136d40,0xc022ce6c,
	0xc0136d44,0xc022ce6c,0xc0136d48,0xc022ce6c,
	0xc0136d4c,0xc022ce6c,0xc0136d78,0xc022ce78,
	0xc01372fc,0xc022ce90,0xc0137314,0xc022ce90,
	0xc0137318,0xc022ce90,0xc0137344,0xc022ce90,
	0xc013735c,0xc022ce90,0xc0137360,0xc022ce90,
	0xc0137388,0xc022ce90,0xc01373c0,0xc022ce90,
	0xc01373c4,0xc022ce90,0xc01373c8,0xc022ce90,
	0xc01373cc,0xc022ce90,0xc0137400,0xc022ce90,
	0xc0137404,0xc022ce90,0xc013741c,0xc022ce90,
	0xc0137430,0xc022ce90,0xc0137484,0xc022ce90,
	0xc01374a0,0xc022ce90,0xc01374a4,0xc022ce90,
	0xc01374a8,0xc022ce90,0xc01374ac,0xc022ce90,
	0xc0137500,0xc022ce90,0xc0137504,0xc022ce90,
	0xc013752c,0xc022ce90,0xc013757c,0xc022ce90,
	0xc0137580,0xc022ce90,0xc0137584,0xc022ce90,
	0xc0137588,0xc022ce90,0xc01375dc,0xc022ce90,
	0xc01375e0,0xc022ce90,0xc0137608,0xc022ce90,
	0xc0137648,0xc022ce90,0xc0137660,0xc022ce90,
	0xc0137664,0xc022ce90,0xc0137668,0xc022ce90,
	0xc013766c,0xc022ce90,0xc01376c0,0xc022ce90,
	0xc01376c4,0xc022ce90,0xc01376ec,0xc022ce90,
	0xc013771c,0xc022ce90,0xc0137930,0xc013795c,
	0xc013793c,0xc013795c,0xc0137940,0xc013795c,
	0xc0137950,0xc013795c,0xc0138b5c,0xc0138b98,
	0xc0138b6c,0xc0138b98,0xc0138b70,0xc0138b98,
	0xc0138b7c,0xc0138b98,0xc0138b88,0xc0138b98,
	0xc0138b8c,0xc0138b98,0xc0138fa8,0xc022cec0,
	0xc0138fe4,0xc022ced0,0xc01418bc,0xc022ced8,
	0xc01629f8,0xc022cee4,0xc0162a00,0xc022cef0,
	0xc0162a18,0xc022cefc,0xc0162a20,0xc022cf08,
	0xc0162a38,0xc022cf14,0xc0162a40,0xc022cf20,
	0xc0162a58,0xc022cf2c,0xc0162a60,0xc022cf38,
	0xc0162a7c,0xc022cf44,0xc0162a84,0xc022cf50,
	0xc0162a98,0xc022cf5c,0xc0162aa0,0xc022cf68,
	0xc0163e08,0xc022cf74,0xc0163e10,0xc022cf80,
	0xc0163e28,0xc022cf8c,0xc0163e30,0xc022cf98,
	0xc0163e48,0xc022cfa4,0xc0163e54,0xc022cfb0,
	0xc0163e64,0xc022cfbc,0xc0163e70,0xc022cfc8,
	0xc0163e80,0xc022cfd4,0xc0163e88,0xc022cfe0,
	0xc01668bc,0xc022cfec,0xc01668c4,0xc022cff4,
	0xc0166b08,0xc022cffc,0xc0166b14,0xc022d004,
	0xc0166b20,0xc022d00c,0xc0166b2c,0xc022d014,
	0xc0166b68,0xc022d01c,0xc0166b74,0xc022d024,
	0xc0166e54,0xc022d02c,0xc0166ee0,0xc022d038,
	0xc0166ee4,0xc022d044,0xc0167078,0xc022d050,
	0xc0167080,0xc022d05c,0xc016708c,0xc022d068,
	0xc0167094,0xc022d074,0xc0167304,0xc022d080,
	0xc016a9c4,0xc022d088,0xc016aa08,0xc022d090,
	0xc01a7838,0xc022d098,0xc01a8690,0xc022d0a0,
	0xc01a86ac,0xc022d0a8,0xc0226b38,0xc022d0b0,
};

void dump_mem(void *start, int size)
{
	int i;
	char * s = start;
	for (i = 0; i < size; i++) {
		printf("0x%02x, ", s[i]&0xff);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
	printf("\n");
}

int main(int argc, char *argv[], char *env[])
{
	__start___ex_table = (struct exception_table_entry *)array;
	__stop___ex_table = __start___ex_table + ARRAY_SIZE(array)/2;
	//printf("%d\n", __stop___ex_table - __start___ex_table);
	dump_mem(__start___ex_table, (unsigned long)__stop___ex_table - (unsigned long)__start___ex_table);
	sort(__start___ex_table, __stop___ex_table - __start___ex_table, sizeof(struct exception_table_entry), cmp_ex, NULL);
	dump_mem(__start___ex_table, (unsigned long)__stop___ex_table - (unsigned long)__start___ex_table);
	return 0;
}
