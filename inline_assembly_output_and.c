/*
to show the difference of & and no &
gcc manual:
¡®&¡¯ Means (in a particular alternative) that this operand is an earlyclobber operand,
which is modified before the instruction is finished using the input operands.
Therefore, this operand may not lie in a register that is used as an input operand
or as part of any memory address.
¡®&¡¯ applies only to the alternative in which it is written. In constraints with
multiple alternatives, sometimes one alternative requires ¡®&¡¯ while others do
not. See, for example, the ¡®movdf¡¯ insn of the 68000.
An input operand can be tied to an earlyclobber operand if its only use as an
input occurs before the early result is written. Adding alternatives of this form
often allows GCC to produce better code when only some of the inputs can be
affected by the earlyclobber. See, for example, the ¡®mulsi3¡¯ insn of the ARM.
¡®&¡¯ does not obviate the need to write ¡®=¡¯.

ARM GCC Inline Assembler Cookbook:
http://www.ethernut.de/en/documents/arm-inline-asm.html

 Constraint characters may be prepended by a single constraint modifier. Constraints without a modifier specify read-only operands. Modifiers are:
Modifier 	Specifies
= 	Write-only operand, usually used for all output operands
+ 	Read-write operand, must be listed as an output operand
& 	A register that should be used for output only 
*/
#include <stdio.h>
typedef struct {
	volatile int counter;
}atomic_t;
/*-Os without &:%0 %2 are the same register:r1
#APP
@ 9 "inline_assembly_output_and.c" 1
	1:	ldrex r1, [r0]
	strex r1, r1, [r0]
	teq r1, #0
	bne 1b
@ 0 "" 2
	bx	lr
*/
void atomic_set(atomic_t *v, int i)
{
	unsigned long tmp;

	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	strex %0, %2, [%1]\n"
"	teq %0, #0\n"
"	bne 1b"
	:"=r"(tmp)
	:"r"(&v->counter),"r"(i)
	:"cc");
}

/*-Os, with &:%0 %2 are different registers, %0:r3 %2:r1
#APP
@ 23 "inline_assembly_output_and.c" 1
	1:	ldrex r3, [r0]
	strex r3, r1, [r0]
	teq r3, #0
	bne 1b
@ 0 "" 2
	bx	lr
*/
void _atomic_set(atomic_t *v, int i)
{
	unsigned long tmp;

	__asm__ __volatile__(
"1:	ldrex %0, [%1]\n"
"	strex %0, %2, [%1]\n"
"	teq %0, #0\n"
"	bne 1b"
	:"=&r"(tmp)
	:"r"(&v->counter),"r"(i)
	:"cc");
}
/*-Os without &, %0 and %1 is the same register: r0
#APP
@ 81 "inline_assembly_output_and.c" 1
	add r0, r0, #128

@ 0 "" 2
	bx	lr
*/
int add(int i)
{
	int temp;
	__asm__ __volatile__(
	"add %0, %1, #128\n"
	:"=r"(temp)
	:"r"(i));
	return temp;
}
/*-Os with &, %0 is different from %1, the former is r3, the latter is r0
#APP
@ 91 "inline_assembly_output_and.c" 1
	add r3, r0, #128

@ 0 "" 2
	mov	r0, r3
	bx	lr
*/
int _add(int i)
{
	int temp;
	__asm__ __volatile__(
	"add %0, %1, #128\n"
	:"=&r"(temp)
	:"r"(i));
	return temp;
}

//borrowed from __xchg, only verify size == 4.

/*
@ 117 "inline_assembly_output_and.c" 1
        @__xchg4
        swp r0, r0, [r1]

@ 0 "" 2
        bx      lr
*/
unsigned long xchg4(unsigned long x, volatile void *ptr)
{
	unsigned long ret;
	__asm__ __volatile__("@__xchg4\n\t"
	"swp %0, %1, [%2]\n\t"
	:"=r"(ret)
	:"r"(x), "r"(ptr)
	:"memory", "cc");
	return ret;
}
/* from the output of -Os, a different reg from 2 input regs is selected(r3), 
   and an extra mov r0,r3 is needed to return proper value.
   It seems that "&" here is unnecessary since we need not keep input parameter
   "x"'s value not changed.
#APP
@ 127 "inline_assembly_output_and.c" 1
        @__xchg4
        swp r3, r0, [r1]

@ 0 "" 2
        mov     r0, r3
        bx      lr
*/
unsigned long _xchg4(unsigned long x, volatile void *ptr)
{
	unsigned long ret;
	__asm__ __volatile__("@__xchg4\n\t"
	"swp %0, %1, [%2]\n\t"
	:"=&r"(ret)
	:"r"(x), "r"(ptr)
	:"memory", "cc");
	return ret;
}

int main()
{
	return 0;
}
//output .s by -Os
#if 0
	.arch armv5te
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 4
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"inline_assembly_output_and.c"
	.text
	.align	2
	.global	atomic_set
	.type	atomic_set, %function
atomic_set:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
#APP
@ 44 "inline_assembly_output_and.c" 1
	1:	ldrex r1, [r0]
	strex r1, r1, [r0]
	teq r1, #0
	bne 1b
@ 0 "" 2
	bx	lr
	.size	atomic_set, .-atomic_set
	.align	2
	.global	_atomic_set
	.type	_atomic_set, %function
_atomic_set:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
#APP
@ 68 "inline_assembly_output_and.c" 1
	1:	ldrex r3, [r0]
	strex r3, r1, [r0]
	teq r3, #0
	bne 1b
@ 0 "" 2
	bx	lr
	.size	_atomic_set, .-_atomic_set
	.align	2
	.global	add
	.type	add, %function
add:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
#APP
@ 88 "inline_assembly_output_and.c" 1
	add r0, r0, #128

@ 0 "" 2
	bx	lr
	.size	add, .-add
	.align	2
	.global	_add
	.type	_add, %function
_add:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
#APP
@ 106 "inline_assembly_output_and.c" 1
	add r3, r0, #128

@ 0 "" 2
	mov	r0, r3
	bx	lr
	.size	_add, .-_add
	.align	2
	.global	xchg4
	.type	xchg4, %function
xchg4:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
#APP
@ 126 "inline_assembly_output_and.c" 1
	@__xchg4
	swp r0, r0, [r1]
	
@ 0 "" 2
	bx	lr
	.size	xchg4, .-xchg4
	.align	2
	.global	_xchg4
	.type	_xchg4, %function
_xchg4:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
#APP
@ 149 "inline_assembly_output_and.c" 1
	@__xchg4
	swp r3, r0, [r1]
	
@ 0 "" 2
	mov	r0, r3
	bx	lr
	.size	_xchg4, .-_xchg4
	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r0, #0
	bx	lr
	.size	main, .-main
	.ident	"GCC: (Buildroot 2013.05) 4.7.3"
	.section	.note.GNU-stack,"",%progbits
#endif
