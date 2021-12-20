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
irq1:
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