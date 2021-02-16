#include <stdio.h>
#include <stdlib.h>
/*
(gdb) p/x (int)0xdeadbeef/16
$8 = 0xfdeadbef<--note singed int 0xdeadbeef /16 is not 0xfdeadbee!
(gdb) p/x (unsigned)0xdeadbeef/16
$9 = 0xdeadbee
(gdb) p/x (int)0xdeadbeef>>4 <- shift behaves the same as /16, no matter if the divided is int or unsigned
$10 = 0xfdeadbee
(gdb) p/x (unsigned)0xdeadbeef>>4
$11 = 0xdeadbee
......
(gdb) p/x (signed)0xffdeadbf % 16<--0xffdeadbf is recognized as unsinged by defauly
$9 = 0xffffffff
(gdb) p (signed)0xffdeadbf % 16
$10 = -1
*/
void krpx(int i)//both / and % will be influenced by signed, as % is actually using /?
{
	printf("\t%08x\n", i);
	if(i) {
		krpx(i / 16);
		printf("%x (%d)\n", i % 16, i % 16);//tab[-1] results in getting wrong char. which is 0 in qemu
	}
}

void krpxu(unsigned int i)
{
	printf("\t%08x\n", i);
	if(i) {
		krpx(i / 16);
		printf("%x-\n", i % 16);
	}
}
/*
./a.out 0x70101010 will work normaly as it is not a "minus int"
./a.out 0xdeadbeef will be wrong
*/
int main(int argc, char *argv[])
{
	int i = 0xdeadbeef;
	if(argc > 1)
		i = strtol(argv[1], NULL, 0);
	printf("%x:\n", i);
	krpx(i);
	krpxu(i);
	return 0;
}
