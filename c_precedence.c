/*
C's Precedence Table
http://www.csee.umbc.edu/courses/104/fall06/burt/precedenceTable.html
*/
#include <stdio.h>

int main()
{
	unsigned long addr[] = {0x1, 0x2};
	unsigned long tmp, *p;
	
	p = addr;
	tmp = *(p++);
	printf("*(p++)\t:addr %p, p %p, tmp 0x%lx\n", addr, p, tmp);
	p = addr;
	tmp = *p++;
	printf("*p++\t:addr %p, p %p, tmp 0x%lx\n", addr, p, tmp);
	return 0;
}