#include <stdio.h>
/*
ARM GCC Inline Assembler Cookbook
http://www.ethernut.de/en/documents/arm-inline-asm.html

Constraints without a modifier specify read-only operands. 

without -Os compiling, the input part by "r"(temp) is:
	ldr	r3, [fp, #-8]
although it is useless.
*/
#define raw_local_irq_enable() \
({ \
	unsigned long temp; \
	__asm__ __volatile__( \
	"mrs %0, cpsr\n\t" \
	"bic %0, %0, #1<<7 @IFTModes\n\t" \
	"msr cpsr_c, %0\n\t" \
	: \
	: "r"(temp)\
	: \
	);\
})
//	r(0x8086) will add ldr	r3, .L2
void mov_imme(void *ptr)
{
	int ret;
	asm volatile(
	"swp %0, %1, [%2]\n\t"
	:"=r"(ret)
	:"r"(0x8086), "r"(ptr));
	asm volatile(
	"swp %0, %1, [%2]\n\t"
	:"=r"(ret)
	:"r"(0x8), "r"(ptr));//will cause mov r3, #8
}
int main()
{
	raw_local_irq_enable();
	return 0; 
}

//output without -Os
#if 0
	.arch armv5te
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"inline_assembly_input.c"
	.text
	.align	2
	.global	mov_imme
	.type	mov_imme, %function
mov_imme:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	stmfd	sp!, {r4, fp}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-16]
	ldr	r3, .L2
	ldr	r2, [fp, #-16]
#APP
@ 28 "inline_assembly_input.c" 1
	swp r4, r3, [r2]
	
@ 0 "" 2
	str	r4, [fp, #-8]
	mov	r3, #8
	ldr	r2, [fp, #-16]
#APP
@ 32 "inline_assembly_input.c" 1
	swp r4, r3, [r2]
	
@ 0 "" 2
	str	r4, [fp, #-8]
	sub	sp, fp, #4
	ldmfd	sp!, {r4, fp}
	bx	lr
.L3:
	.align	2
.L2:
	.word	32902
	.size	mov_imme, .-mov_imme
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	ldr	r3, [fp, #-8]
#APP
@ 39 "inline_assembly_input.c" 1
	mrs r3, cpsr
	bic r3, r3, #1<<7 @IFTModes
	msr cpsr_c, r3
	
@ 0 "" 2
	mov	r3, #0
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	main, .-main
	.ident	"GCC: (Buildroot 2013.05) 4.7.3"
	.section	.note.GNU-stack,"",%progbits
#endif