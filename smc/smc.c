#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COUNT 10000
char buf[1024*1024];
//char *__buf = buf[0];
/*
https://stackoverflow.com/questions/4156585/how-to-get-the-length-of-a-function-in-bytes
You can do this without a custom linker script by calling the section by a valid C identifier name, for example mysection instead of .mysection, and using __start_mysection and __stop_mysection instead of mysec_start and mysec_end. The linker automatically defines these symbols for each section encountered
*/
#if 0//__attribute__ can NOT be put after function name
smc.c:8:2: error: attributes should be specified before the declarator in a function definition
  void f1()__attribute__((section("mysection")))
  ^~~~

#endif
__attribute__((section("mysection1"))) void f1()
{
	strcat(buf, __func__);
	strcat(buf, "\n");
}

__attribute__((section("mysection2"))) void f2()
{
	strcat(buf, __func__);
	strcat(buf, "\n");
}

typedef void (*func)(void);
int main()
{
/*readelf -a a.out
    46: 000000000000074c     0 NOTYPE  GLOBAL PROTECTED   15 __stop_mysection
    47: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_deregisterTMCloneTab
    48: 0000000000201000     0 NOTYPE  WEAK   DEFAULT   24 data_start
    49: 0000000000000732     0 NOTYPE  GLOBAL PROTECTED   15 __start_mysection
*/
	/*wrong way 1:__stop_mysection/__start_mysection declared like below will not be address but the value of address!
//8ec8353 53e58955, 0x565df664 0x565df632, -1257833986
	extern unsigned long __stop_mysection, __start_mysection;
	printf("%lx %lx, %p %p, %ld\n", __stop_mysection, __start_mysection, &__stop_mysection, &__start_mysection ,__stop_mysection - __start_mysection);
	*/
	/*wrong way 2:__stop_mysection-__start_mysection is 12 not 50, for it is uint*-uint*, not char*-char*
	extern unsigned int __stop_mysection[], __start_mysection[];
//0x565d4664 0x565d4632, 8ec8353 53e58955, 12<--50B/4==12, which is not the expected result of function bytes size
	printf("%p %p, %x %x, %d\n", __stop_mysection, __start_mysection, *__stop_mysection, *__start_mysection, __stop_mysection - __start_mysection);
	*/
	extern char __start_mysection1[], __stop_mysection1[];
	extern char __start_mysection2[], __stop_mysection2[];
	char *p;
	func f;
	int f1_size = __stop_mysection1 - __start_mysection1;
	int f2_size = __stop_mysection2 - __start_mysection2;
	int i, max = f1_size > f2_size? f1_size: f2_size;
	p = malloc(max);
	f = (func)p;
	printf("%d %d: %d, %p\n", f1_size, f2_size, max, p);
	for (i = 0; i < 32; i++) {
		char *start, *end;
		start = __start_mysection1;
		end = __stop_mysection1;
		if ((i + 1) % 2) {
			start = __start_mysection2;
			end = __stop_mysection2;
		}
		printf("%p %p\n", start, end);
		memcpy(p, start, end - start);
		(*f)();
	}
	printf("[%s]", buf);
	return 0;
}