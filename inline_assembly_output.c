#include <stdio.h>
/*
"=" will make an output, although it is useless.
	str	r4, [fp, #-8]
*/
#define raw_local_irq_enable() \
({ \
	unsigned long temp; \
	__asm__ __volatile__( \
	"mrs %0, cpsr\n\t" \
	"bic %0, %0, #1<<7 @IFTModes\n\t" \
	"msr cpsr_c, %0\n\t" \
	: "=r"(temp)\
	: \
	: "memory"\
	);\
})
int main()
{
	raw_local_irq_enable();
	return 0; 
}

//output without -Os compiling
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
	.file	"inline_assembly_output.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	stmfd	sp!, {r4, fp}
	add	fp, sp, #4
	sub	sp, sp, #8
#APP
@ 16 "inline_assembly_output.c" 1
	mrs r4, cpsr
	bic r4, r4, #1<<7 @IFTModes
	msr cpsr_c, r4
	
@ 0 "" 2
	str	r4, [fp, #-8]
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {r4, fp}
	bx	lr
	.size	main, .-main
	.ident	"GCC: (Buildroot 2013.05) 4.7.3"
	.section	.note.GNU-stack,"",%progbits
#endif	
