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
3.kernel's lib/vsprintf.c 's depend still has gcc lib's stdarg.h:
cmd_lib/vsprintf.o := arm-none-linux-gnueabi-gcc -Wp,-MD,lib/.vsprintf.o.d  -nostdinc -isystem /home/rednoah/opt/src/mini2440/arm-2008q3/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/include -Iinclude -Iinclude2 -I/home/rednoah/opt/src/mini2440/kernel/mini2440/include -I/home/rednoah/opt/src/mini2440/kernel/mini2440/arch/arm/include -include include/linux/autoconf.h  -I/home/rednoah/opt/src/mini2440/kernel/mini2440/lib -Ilib -D__KERNEL__ -mlittle-endian   -I/home/rednoah/opt/src/mini2440/kernel/mini2440/arch/arm/mach-s3c2410/include   -I/home/rednoah/opt/src/mini2440/kernel/mini2440/arch/arm/mach-s3c2400/include   -I/home/rednoah/opt/src/mini2440/kernel/mini2440/arch/arm/mach-s3c2412/include   -I/home/rednoah/opt/src/mini2440/kernel/mini2440/arch/arm/mach-s3c2440/include   -I/home/rednoah/opt/src/mini2440/kernel/mini2440/arch/arm/mach-s3c2442/include   -I/home/rednoah/opt/src/mini2440/kernel/mini2440/arch/arm/mach-s3c2443/include   -I/home/rednoah/opt/src/mini2440/kernel/mini2440/arch/arm/plat-s3c24xx/include   -I/home/rednoah/opt/src/mini2440/kernel/mini2440/arch/arm/plat-s3c/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables -D__LINUX_ARM_ARCH__=4 -march=armv4t -mtune=arm920t -msoft-float -Uarm -fno-stack-protector -fomit-frame-pointer -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(vsprintf)"  -D"KBUILD_MODNAME=KBUILD_STR(vsprintf)"  -c -o lib/vsprintf.o /home/rednoah/opt/src/mini2440/kernel/mini2440/lib/vsprintf.c

deps_lib/vsprintf.o := \
  /home/rednoah/opt/src/mini2440/kernel/mini2440/lib/vsprintf.c \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/binary/printf.h) \
  /home/rednoah/opt/src/mini2440/arm-2008q3/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/include/stdarg.h \<======
  ....
4.Example for -nostdinc & -isystem
divide.c only includes one header: stdarg.h that comes from gcc’s lib, not from libc:
arm$ find ~/opt/src/mini2440/arm-2008q3/ -name stdarg.h|xargs md5sum
faa4936c0a0220f733882c65b5ef60b8  /home/rednoah/opt/src/mini2440/arm-2008q3/arm-none-linux-gnueabi/include/c++/4.3.2/tr1/stdarg.h
10c9f28806d14aba38914b97ca1ba7d1  /home/rednoah/opt/src/mini2440/arm-2008q3/lib/gcc/arm-none-linux-gnueabi/4.3.2/include/stdarg.h
The 1st is libc, the 2nd is gcc’s own lib
arm$ arm-none-linux-gnueabi-gcc -M divide.cuse gcc’s stdarg.h, not libc’s stdarg.h
divide.o: divide.c \
  /home/rednoah/opt/src/mini2440/arm-2008q3/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/include/stdarg.h
arm$ arm-none-linux-gnueabi-gcc -M divide.c -nostdincproduce error as -nostdinc specified
divide.c:11:20: error: no include path in which to search for stdarg.h
after adding -isystem, it becomes ok, the following ‘-isystem’ option is from kernel's lib/vsprintf.c building command:
arm$ arm-none-linux-gnueabi-gcc -M divide.c -nostdinc -isystem /home/rednoah/opt/src/mini2440/arm-2008q3/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/include
divide.o: divide.c \
  /home/rednoah/opt/src/mini2440/arm-2008q3/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/include/stdarg.h
The dir after -isystem is used as ‘standard system dirs’ to be searched after all -I option and before the default ‘standard system dirs’. the default ‘standard system dirs’ are disabled by ‘-nostdinc’. But actually the disabled default 'standard system dirs' are the same as the dirs specified by '-isystem' in the example above.
kernel's lib/vsprintf.c’s dependant stdarg.h is exactly the same of the above gcc’s stdarg.h

*/

typedef unsigned long pgd_t[2];
void func(pgd_t*, unsigned long*, unsigned long);
pgd_t *pgd;// = (void*)-1ULL;
unsigned long *pmd;
/*
test:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	mov	r1, #-1073741824
	push	{r4, lr}
	ldr	r3, .L5
	str	r1, [r3]
	ldr	r3, .L5+4
	mov	r0, r1
	ldr	r2, [r1]
	str	r1, [r3]
	bl	func
	pop	{r4, lr}
	bx	lr
.L6:
	.align	2
.L5:
	.word	pgd
	.word	pmd
	.size	test, .-test
	.comm	pmd,4,4
	.comm	pgd,4,4
--readelf -s--
    13: 00000004     4 OBJECT  GLOBAL DEFAULT  COM pgd
    14: 00000004     4 OBJECT  GLOBAL DEFAULT  COM pmd*/
void test()
{
	pgd = (void*)0xc0000000;
	pmd = (void*)pgd;//if no (void*):warning: assignment to 'long unsigned int *' from incompatible pointer type 'long unsigned int (*)[2]' [-Wincompatible-pointer-types]
	func(pgd, pgd[0], pmd[0]);
/*the former 2 are of the same value; both @pmd&@pgd are pointer but their reference has different explanation by gcc, note
in elf symbol table these 2 are exactly the same(see above symbol table). It is gcc that is responsible for explain their *.
pgd[0]:
(gdb) ptype pgd[0]
type = long unsigned int [2]<=
(gdb) ptype pgd
type = long unsigned int *)[2]
pmd[0]:
(gdb) ptype pmd
type = long unsigned int *
(gdb) ptype pmd[0]
type = long unsigned int<=

*/
}