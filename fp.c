int main()
{
	func0();
	return 0;
}
#if 0
@arm-linux-gcc -fno-omit-frame-pointer -Os  -S fp.c -o fp.s
@it will not reserve like "struct stackframe" of kernel
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	bl	func0
	mov	r0, #0
	ldmfd	sp!, {fp, pc}
	.size	main, .-main
	.ident	"GCC: (Buildroot 2013.05) 4.7.3"
	.section	.note.GNU-stack,"",%progbits
	
@arm-linux-gcc -fomit-frame-pointer -Os -S fp.c -o fp_omit.s
@fp is not reserved at all
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r3, lr}
	bl	func0
	mov	r0, #0
	ldmfd	sp!, {r3, pc}
	.size	main, .-main
	.ident	"GCC: (Buildroot 2013.05) 4.7.3"
	.section	.note.GNU-stack,"",%progbits
	
@arm-linux-gcc -fno-omit-frame-pointer -Os -mapcs -S fp.c -o fp_mapcs.s
@the same as kernel way: store "struct stackframe" at the high end of current stacke frame
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	bl	func0
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
	.size	main, .-main
	.ident	"GCC: (Buildroot 2013.05) 4.7.3"
	.section	.note.GNU-stack,"",%progbits
	
#endif