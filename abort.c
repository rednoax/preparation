#include <unistd.h>
#include <stdio.h>
//gcc -Wall abort.c -Wl,-verbose to show ld script when linking

//http://blog.linux.org.tw/~jserv/archives/002030.html
extern int __bss_start, _end;

char buf[12];
/*
[root@localhost preparation]# cat /proc/14140/maps
08048000-08049000 r-xp 00000000 08:01 540345     /root/preparation/a.out
08049000-0804a000 r--p 00000000 08:01 540345     /root/preparation/a.out
0804a000-0804b000 rw-p 00001000 08:01 540345     /root/preparation/a.out
099e4000-09a05000 rw-p 00000000 00:00 0          [heap]
b752c000-b76de000 r-xp 00000000 08:01 2100389    /usr/lib/libc-2.24.so
b76de000-b76df000 ---p 001b2000 08:01 2100389    /usr/lib/libc-2.24.so
b76df000-b76e1000 r--p 001b2000 08:01 2100389    /usr/lib/libc-2.24.so
b76e1000-b76e2000 rw-p 001b4000 08:01 2100389    /usr/lib/libc-2.24.so
b76e2000-b76e5000 rw-p 00000000 00:00 0
b76f4000-b76f6000 rw-p 00000000 00:00 0
b76f6000-b76f8000 r--p 00000000 00:00 0          [vvar]
b76f8000-b76fa000 r-xp 00000000 00:00 0          [vdso]
b76fa000-b771c000 r-xp 00000000 08:01 2100388    /usr/lib/ld-2.24.so
b771c000-b771d000 r--p 00021000 08:01 2100388    /usr/lib/ld-2.24.so
b771d000-b771e000 rw-p 00022000 08:01 2100388    /usr/lib/ld-2.24.so
bfce8000-bfd09000 rw-p 00000000 00:00 0          [stack]
*/
int main()
{
	unsigned long __end = (unsigned long)&_end;
	unsigned long proc_maps_end = (__end + (1<<12)) & ~((1<<12)-1);
	char * p = (char*)proc_maps_end;
	p[-1] = '0';
	printf("%d %lx %lx:%x\n", getpid(), __end, proc_maps_end, p[-1]);
	//p[0] = '1';//segmentation fault(core dumped)
	sleep(100);
	return 0;
}