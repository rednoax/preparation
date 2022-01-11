/*
different -mabi option causes different divide function name!
if no -mabi, aapcs-linux is used
arm$ arm-none-linux-gnueabi-gcc -S -mabi=aapcs-linux divide.c -o 2.s <= bl	__aeabi_idiv
arm$ arm-none-linux-gnueabi-gcc -S -mabi=apcs-gnu divide.c -o 1.s <= bl	__divsi3
*/
int divide(int i)
{
	return i / *(int*)4;
}
#include <stdarg.h>
/*
1. no need to -I
arm$ arm-none-eabi-gcc -c divide.c -M
divide.o: divide.c /usr/lib/gcc/arm-none-eabi/9.2.1/include/stdarg.h
2. u-boot's vsprintf.c command line:
/home/rednoah/opt/src/mini2440/arm-2008q3/bin/arm-none-linux-gnueabi-gcc -g  -Os   -fno-strict-aliasing  -fno-common -ffixed-r8 -msoft-float  -D__KERNEL__ -DTEXT_BASE=0x33F80000 -I/home/rednoah/opt/src/mini2440/uboot/mini2440/include -fno-builtin -ffreestanding -nostdinc -isystem /home/rednoah/opt/src/mini2440/arm-2008q3/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/include -pipe  -DCONFIG_ARM -D__ARM__ -march=armv4t -mabi=apcs-gnu -mno-thumb-interwork -Wall -Wstrict-prototypes -c -o vsprintf.o vsprintf.c
*/

typedef unsigned long pgd_t[2];
void func(pgd_t*, unsigned long*);
pgd_t *pgd;// = (void*)-1ULL;
/*
test:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	mov	r1, #-1073741824
	push	{r4, lr}
	ldr	r3, .L5
	mov	r0, r1
	str	r1, [r3]
	bl	func
	pop	{r4, lr}
	bx	lr
*/
void test()
{
	pgd = (void*)0xc0000000;
	func(pgd, pgd[0]);//these 2 are of the same value
}