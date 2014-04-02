#include <stdlib.h>
#include <stdio.h>
#define atomic_add(ptr, val) \
	({ register unsigned int *__ptr asm("r2") = (ptr); \
	   register unsigned int __result asm("r1"); \
	   asm volatile ( \
	       "1: @ atomic_add\n\t" \
	       "ldr	r0, [r2]\n\t" \
	       "mov	r3, #0xffff0fff\n\t" \
	       "add	lr, pc, #4\n\t" \
	       "add	r1, r0, %2\n\t" \
	       "add	pc, r3, #(0xffff0fc0 - 0xffff0fff)\n\t" \
	       "bcc	1b" \
	       : "=&r" (__result) \
	       : "r" (__ptr), "rIL" (val) \
	       : "r0","r3","ip","lr","cc","memory" ); \
	   __result; })

unsigned int test(unsigned long arg)
{
	unsigned int org = 0, result;
	__asm__ __volatile__(
	"mov	%2, #0xffff0fff\n\t"
	"sub	%2, %2, #(0xffff0fff - 0xffff0fc0)\n\t"
	"subs	%0, %2, %1\n\t"
	:"=r"(result)
	:"r"(arg), "r"(org)
	:"cc", "memory");
	asm volatile("start:");
	/*
	return atomic_add(&result, 0xff);
#if 0//-Os
	00008310 <start>:
    8310:       e5920000        ldr     r0, [r2]
    8314:       e3e03a0f        mvn     r3, #61440      ; 0xf000
    8318:       e28fe004        add     lr, pc, #4
    831c:       e28010ff        add     r1, r0, #255    ; 0xff
    8320:       e243f03f        sub     pc, r3, #63     ; 0x3f
    8324:       3afffff9        bcc     8310 <start>
    8328:       e1a00001        mov     r0, r1
    832c:       e8bd800e        pop     {r1, r2, r3, pc}
#endif	
	*/
	atomic_add(&result, 0xff);
	asm volatile("end:");
#if 0//the above with -Os will generate: 
00008310 <start>:
    8310:       e5920000        ldr     r0, [r2]
    8314:       e3e03a0f        mvn     r3, #61440      ; 0xf000
    8318:       e28fe004        add     lr, pc, #4
    831c:       e28010ff        add     r1, r0, #255    ; 0xff
    8320:       e243f03f        sub     pc, r3, #63     ; 0x3f
    8324:       3afffff9        bcc     8310 <start>

00008328 <end>:
    8328:       e1a00004        mov     r0, r4
    832c:       e8bd801c        pop     {r2, r3, r4, pc}
#endif	
	return 0;
}
int main(int argc, char **argv)
{
	/*
	1.
	compile:
	arm-linux-gcc -Wall -static -g xx.c
	run:
	qemu-arm -g 1234 -cpu arm926 a.out 1 2 3 will print:
	0:a.out
	1:1
	2:2
	3:3
	*/
	int i;
	for (i = 0; i < argc; i++)
		printf("%d:%s\n", i, argv[i]);
	if (argc > 1) {
		//drop strtol since it will trans 0xffff0fc1 to 0x7fffffff
		unsigned long arg = strtoul(argv[1], NULL, 0);
		printf("test %lx, %ld\n", arg, (long)arg);
		test(arg);
	}
	return 0;
}