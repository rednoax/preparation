	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"global.c"
	.comm	g_b,4,4
	.global	g_d
	.data
	.align	2
	.type	g_d, %object
	.size	g_d, 4
g_d:
	.word	1
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	ldr	r3, .L3
	ldr	r2, [r3]
	ldr	r3, .L3+4
	ldr	r3, [r3]
	add	r3, r2, r3
	mov	r0, r3
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
.L4:
	.align	2
.L3:
	.word	g_b
	.word	g_d
	.size	main, .-main
	.ident	"GCC: (Sourcery CodeBench Lite 2013.11-24) 4.8.1"
