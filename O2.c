/*
an example to show O2 that is used in __tlb_flag of kernel
gcc -Wall O2.c
/tmp/ccvD8rcl.o: In function `main':
main.c:(.text+0x34): undefined reference to `_arg'
collect2: error: ld returned 1 exit status
gcc -O2 -Wall O2.c or -O1 will not report error!
*/
#include <stdlib.h>
extern unsigned int _arg;
int main(int argc, char** argv)
{
	int arg = 0;
	if (argc > 1)
		arg = atoi(argv[1]);
	if ( arg || (_arg & arg))
		return -1; 
	return 0;
}
