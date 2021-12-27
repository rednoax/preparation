/*
arm-none-linux-gnueabi-gcc -Os -S -Wall irq.c
simplified result:
irq0:
	sub	lr, lr, #4
	stmfd	sp!, {r0, r1, r2, r3, ip, lr}
	bl	undefined_func
	ldmfd	sp!, {r0, r1, r2, r3, ip, pc}^
irq:
    bx lr
irq1:<=no {r0-r3,ip,lr} saved as they will not be clobbered due to no further call
	subs	pc, lr, #4 @Rd==pc so s means cpsr=spsr, pc=lr-4
*/
extern int undefined_func();
/*
u can specify the kind of interrupt to be handled by adding an optional parameter to
the interrupt attribute like this:
void f() __attribute__((interrupt ("IRQ")));
Permissible values for this parameter are: IRQ, FIQ, SWI, ABORT and UNDEF.
*/
void __attribute__((interrupt)) irq0()
{
    undefined_func();
}

__attribute__((interrupt)) void irq1()
{

}

void irq()
{

}

/* arm-none-eabi-gcc -Wall -Os -S irq.c
	.cpu arm7tdmi
	......
	sub	lr, lr, #4
	push	{r0, r1, r2, r3, ip, lr}<= these 6 regs can be clobbered after preserved
	mov	r3, #1241513984 <=r3, r2, r0, lr clobbered below
	ldr	r2, [r3, #20]
	ldr	r3, .L7
	mov	r0, #0
	ldr	r3, [r3, r2, lsl #2]
	mov	lr, pc
	bx	r3
	ldmfd	sp!, {r0, r1, r2, r3, ip, pc}^
.L8:
	.align	2
.L7:
	.word	HandleEINT0
*/
typedef void* (*func)(void*);
extern func HandleEINT0[32];
#define INTOFFSET ((volatile int*)0x4a000014)
void __attribute__((interrupt("IRQ"))) irq2()
{
	int offset = *INTOFFSET;
	func p = HandleEINT0[offset];
	p(0);
}
#if 0
void __attribute__((noinline)) handler(int i)
{
	*(volatile int*)0xc0000000 = i;
}

void /*__attribute__((interrupt("IRQ")))*/ irq3()
{
	handler(*(volatile int*)0x0);
}
#endif