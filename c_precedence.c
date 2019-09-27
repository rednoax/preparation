/*
C's Precedence Table
http://www.csee.umbc.edu/courses/104/fall06/burt/precedenceTable.html
--
rednoah@ubuntu:~/preparation$ gcc -Wall -m32 c_precedence.c
rednoah@ubuntu:~/preparation$ ./a.out
*(p++)  :addr 0xff8e0ad4, p 0xff8e0ad8, tmp 0x1
*p++    :addr 0xff8e0ad4, p 0xff8e0ad8, tmp 0x1
*(p++)=0        :addr 0xff8e0ad4, 0xff8e0ad8, addr[]:0 2
*/
#include <stdio.h>

int main()
{
	unsigned long addr[] = {0x1, 0x2};
	unsigned long tmp, *p;
	
	p = addr;
	tmp = *(p++);//is exactly equal to *p++
	printf("*(p++)\t:addr %p, p %p, tmp 0x%lx\n", addr, p, tmp);
	p = addr;
	tmp = *p++;
	printf("*p++\t:addr %p, p %p, tmp 0x%lx\n", addr, p, tmp);
	p = addr;
	*(p++) = 0;//equal to "*p++=0" so addr[0] changed but [1] reserved.
	printf("*(p++)=0\t:addr %p, %p, addr[]:%lx %lx\n", addr, p, addr[0], addr[1]);
	return 0;
}
