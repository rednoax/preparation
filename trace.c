/*
extracted from gcc 4.7.2 manual:
[Built-in Function] void * __builtin_return_address (unsigned int level)
This function returns the return address of the current function, or of one of its callers.<--return address of one of its callers
The level argument is number of frames to scan up the call stack. A value of 0 yields
the return address of the current function, a value of 1 yields the return address of
the caller of the current function, and so forth. When inlining the expected behavior
is that the function will return the address of the function that will be returned to.
To work around this behavior use the noinline function attribute.
The level argument must be a constant integer.
On some machines it may be impossible to determine the return address of any
function other than the current one; in such cases, or when the top of the stack has
been reached, this function will return 0 or a random value. In addition, __builtin_
frame_address may be used to determine if the top of the stack has been reached.
Additional post-processing of the returned value may be needed, see __builtin_
extract_return_address.
This function should only be used with a nonzero argument for debugging purposes.
*/
#if 0
FIXME:
1. inline usage:gcc -m32 -O0 cannot inline;but -O1 implement inline:
 80483fd:       66 90                   xchg   %ax,%ax
 80483ff:       90                      nop

08048400 <func0>:
 8048400:       55                      push   %ebp
 8048401:       89 e5                   mov    %esp,%ebp
 8048403:       83 ec 28                sub    $0x28,%esp
 8048406:       8b 45 04                mov    0x4(%ebp),%eax
 8048409:       8b 55 00                mov    0x0(%ebp),%edx
 804840c:       8b 52 04                mov    0x4(%edx),%edx
 804840f:       89 44 24 14             mov    %eax,0x14(%esp)
 8048413:       89 54 24 10             mov    %edx,0x10(%esp)
 8048417:       89 44 24 0c             mov    %eax,0xc(%esp)
 804841b:       c7 44 24 08 00 84 04    movl   $0x8048400,0x8(%esp)
 8048422:       08 
 8048423:       c7 44 24 04 8e 85 04    movl   $0x804858e,0x4(%esp)
 804842a:       08 
 804842b:       c7 04 24 30 85 04 08    movl   $0x8048530,(%esp)
 8048432:       e8 99 fe ff ff          call   80482d0 <printf@plt>
 8048437:       c9                      leave  
 8048438:       c3                      ret    

08048439 <func1>:
 8048439:       83 ec 1c                sub    $0x1c,%esp
 804843c:       8b 44 24 1c             mov    0x1c(%esp),%eax
 8048440:       89 44 24 0c             mov    %eax,0xc(%esp)
 8048444:       c7 44 24 08 39 84 04    movl   $0x8048439,0x8(%esp)
 804844b:       08 
 804844c:       c7 44 24 04 88 85 04    movl   $0x8048588,0x4(%esp)
 8048453:       08 
 8048454:       c7 04 24 68 85 04 08    movl   $0x8048568,(%esp)
 804845b:       e8 70 fe ff ff          call   80482d0 <printf@plt>
 8048460:       e8 9b ff ff ff          call   8048400 <func0>
 8048465:       83 c4 1c                add    $0x1c,%esp<---the return address of inline_func
 8048468:       c3                      ret    

 [root@localhost preparation]# ./a.out 
main:0x8048469
func1:0x8048439, caller0:8048493
func0:0x8048400, caller0:8048465 caller1:f76259d3, inline_func return 8048465

2. only tested on x86, arm maybe need the following when compiling:
from arch/arm/Makefile
ifeq ($(CONFIG_FRAME_POINTER),y)
KBUILD_CFLAGS	+=-fno-omit-frame-pointer -mapcs -mno-sched-prolog
endif
also check include/linux/ftrace.h:
#ifdef CONFIG_FRAME_POINTER
/* TODO: need to fix this for ARM */
# define CALLER_ADDR0 ((unsigned long)__builtin_return_address(0))
# define CALLER_ADDR1 ((unsigned long)__builtin_return_address(1))
# define CALLER_ADDR2 ((unsigned long)__builtin_return_address(2))
# define CALLER_ADDR3 ((unsigned long)__builtin_return_address(3))
# define CALLER_ADDR4 ((unsigned long)__builtin_return_address(4))
# define CALLER_ADDR5 ((unsigned long)__builtin_return_address(5))
# define CALLER_ADDR6 ((unsigned long)__builtin_return_address(6))
#else
# define CALLER_ADDR0 ((unsigned long)__builtin_return_address(0))
# define CALLER_ADDR1 0UL
# define CALLER_ADDR2 0UL
# define CALLER_ADDR3 0UL
# define CALLER_ADDR4 0UL
# define CALLER_ADDR5 0UL
# define CALLER_ADDR6 0UL
#endif
#endif
#define CALLER_ADDR0 ((unsigned long)__builtin_return_address(0))//the return address of the current function
#define CALLER_ADDR1 ((unsigned long)__builtin_return_address(1))
#include <stdio.h>

//static
inline unsigned long inline_func()
{
	return CALLER_ADDR0;
}

void func0()
{
	unsigned long l = inline_func();
	printf("%s:%p, caller0:%lx caller1:%lx, inline_func return %lx\n", __func__, func0, CALLER_ADDR0, CALLER_ADDR1, l);
	
}

void func1()
{
	printf("%s:%p, caller0:%lx\n", __func__, func1, CALLER_ADDR0);
	func0();
}

int main()
{
	int i, j = 4;

	printf("%s:%p\n", __func__, main);
	func1();

	i = __builtin_constant_p(j);
	j = __builtin_constant_p(0);
	/*
	-O:1 1 0 1 1
	no -O and add static before inline_func::0 1 0 0 0
	*/
	printf("__builtin_constant_p %d %d %d %d %d\n", i, j, __builtin_constant_p(main), __builtin_constant_p(i), __builtin_constant_p(j));
	return 0;
}
#if 0
//output of gcc -m32
main:0x8048467
func1:0x8048437, caller0:8048491
func0:0x8048400, caller0:8048465 caller1:8048491

//objdump the elf generated by "gcc -m32"
a.out:     file format elf32-i386


Disassembly of section .interp:
...

Disassembly of section .text:
...
08048400 <func0>:
 8048400:	55                   	push   %ebp
 8048401:	89 e5                	mov    %esp,%ebp
 8048403:	83 ec 28             	sub    $0x28,%esp
 8048406:	8b 45 00             	mov    0x0(%ebp),%eax
 8048409:	8b 40 04             	mov    0x4(%eax),%eax
 804840c:	89 c2                	mov    %eax,%edx
 804840e:	8b 45 04             	mov    0x4(%ebp),%eax
 8048411:	89 54 24 10          	mov    %edx,0x10(%esp)
 8048415:	89 44 24 0c          	mov    %eax,0xc(%esp)
 8048419:	c7 44 24 08 00 84 04 	movl   $0x8048400,0x8(%esp)
 8048420:	08 
 8048421:	c7 44 24 04 6b 85 04 	movl   $0x804856b,0x4(%esp)
 8048428:	08 
 8048429:	c7 04 24 30 85 04 08 	movl   $0x8048530,(%esp)
 8048430:	e8 9b fe ff ff       	call   80482d0 <printf@plt>
 8048435:	c9                   	leave  
 8048436:	c3                   	ret    

08048437 <func1>:
 8048437:	55                   	push   %ebp
 8048438:	89 e5                	mov    %esp,%ebp
 804843a:	83 ec 18             	sub    $0x18,%esp
 804843d:	8b 45 04             	mov    0x4(%ebp),%eax
 8048440:	89 44 24 0c          	mov    %eax,0xc(%esp)
 8048444:	c7 44 24 08 37 84 04 	movl   $0x8048437,0x8(%esp)
 804844b:	08 
 804844c:	c7 44 24 04 71 85 04 	movl   $0x8048571,0x4(%esp)
 8048453:	08 
 8048454:	c7 04 24 50 85 04 08 	movl   $0x8048550,(%esp)
 804845b:	e8 70 fe ff ff       	call   80482d0 <printf@plt>
 8048460:	e8 9b ff ff ff       	call   8048400 <func0>
 8048465:	c9                   	leave  ###########here is CALLER_ADDR0 displayed by func0
 8048466:	c3                   	ret    

08048467 <main>:
 8048467:	55                   	push   %ebp
 8048468:	89 e5                	mov    %esp,%ebp
 804846a:	83 e4 f0             	and    $0xfffffff0,%esp
 804846d:	83 ec 10             	sub    $0x10,%esp
 8048470:	c7 44 24 08 67 84 04 	movl   $0x8048467,0x8(%esp)
 8048477:	08 
 8048478:	c7 44 24 04 77 85 04 	movl   $0x8048577,0x4(%esp)
 804847f:	08 
 8048480:	c7 04 24 64 85 04 08 	movl   $0x8048564,(%esp)
 8048487:	e8 44 fe ff ff       	call   80482d0 <printf@plt>
 804848c:	e8 a6 ff ff ff       	call   8048437 <func1>
 8048491:	b8 00 00 00 00       	mov    $0x0,%eax #######here is CALLER_ADDR0 displayed by func1
 8048496:	c9                   	leave  
 8048497:	c3                   	ret    
 8048498:	66 90                	xchg   %ax,%ax
 804849a:	66 90                	xchg   %ax,%ax
 804849c:	66 90                	xchg   %ax,%ax
 804849e:	66 90                	xchg   %ax,%ax

080484a0 <__libc_csu_init>:
 80484a0:	55                   	push   %ebp
	...
#endif
