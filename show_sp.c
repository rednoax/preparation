#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
/*
this program proves that user/svc sp are 2 different registers
$arm-linux-gnueabihf-gcc -Wall show_sp.c

# ./a.out<--in op-tee armv7-A qemu
sp 7ed2fc68 <--in stack range below
00010000-00011000 r-xp 00000000 00:14 34746901   /mnt/host/preparation/a.out
00020000-00021000 r--p 00000000 00:14 34746901   /mnt/host/preparation/a.out
00021000-00022000 rw-p 00001000 00:14 34746901   /mnt/host/preparation/a.out
00022000-00043000 rw-p 00000000 00:00 0          [heap]
76ec9000-76fa8000 r-xp 00000000 00:02 149        /lib/libc-2.31.so
76fa8000-76fb8000 ---p 000df000 00:02 149        /lib/libc-2.31.so
76fb8000-76fba000 r--p 000df000 00:02 149        /lib/libc-2.31.so
76fba000-76fbb000 rw-p 000e1000 00:02 149        /lib/libc-2.31.so
76fbb000-76fbe000 rw-p 00000000 00:00 0
76fbe000-76fd7000 r-xp 00000000 00:02 143        /lib/ld-2.31.so
76fe5000-76fe7000 rw-p 00000000 00:00 0
76fe7000-76fe8000 r--p 00019000 00:02 143        /lib/ld-2.31.so
76fe8000-76fe9000 rw-p 0001a000 00:02 143        /lib/ld-2.31.so
7ed0f000-7ed30000 rw-p 00000000 00:00 0          [stack]
7ef89000-7ef8a000 r-xp 00000000 00:00 0          [sigpage]
7ef8a000-7ef8b000 r--p 00000000 00:00 0          [vvar]
7ef8b000-7ef8c000 r-xp 00000000 00:00 0          [vdso]
ffff0000-ffff1000 r-xp 00000000 00:00 0          [vectors]
*/
int main()
{
	int sp;
	char buf[128];
	asm volatile(
		"mov %0, sp\n\t"
		:"=r"(sp));
	printf("sp %x\n", sp);
	sprintf(buf, "cat /proc/%d/maps", getpid());
	system(buf);
	pause();
	return 0;
}
