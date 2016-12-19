#include <stdlib.h>
#include <stdio.h>

unsigned long addr_limit = 0xbf000000;
int __range_ok(addr, size)
{
	unsigned long flag, roksum;
	__asm__ __volatile__(
	"adds %1, %2, %3\n"
	"sbcccs %1, %1, %0\n"
	"movcc %0, #0"
	:"=&r"(flag), "=&r"(roksum)
	:"r"(addr), "Ir"(size), "0"(addr_limit)
	:"cc");
	return flag;
}

#define access_ok(type, addr, size) (__range_ok(addr, size) == 0)
//qemu-arm a.out to test it directly
int main(int argc, char **argv)
{
	const char *p = (const char*)(0xbf000000 - 1);
	int ret;
	if (argc > 1)
		addr_limit = strtoul(argv[1], NULL, 0);
	printf("addr_limit %lx\n", addr_limit);
	ret = access_ok(0, p, 1);
	printf("%p access_ok 1B %d\n", p, ret);
	p = (const char*)0xffffffff;
	ret = access_ok(0, p, 1);
	printf("%p access_ok 1B %d\n", p, ret);
	p = (const char*)(0xbf000000);
	ret = access_ok(0, p, 1);
	printf("%p access_ok 1B %d\n", p , ret);
	return 0;
}