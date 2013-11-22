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
	.file	"inline_assembly.c"
	.text
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
@ 16 "inline_assembly.c" 1
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
