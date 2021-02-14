int g_b;
int g_d = 1;
/*
arm-none-eabi-gcc -S global.c
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	ldr	r3, .L3<--get global symbol g_b's value, which is an address in .bss
	ldr	r2, [r3]<--to refer its value in .bss
	ldr	r3, .L3+4<--get global symbol g_d's value, which is an address in .data
	ldr	r3, [r3]<--to refer its value in .data
	add	r3, r2, r3
	mov	r0, r3
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
.L4:
	.align	2
.L3:
	.word	g_b<--32bit symbol value:address in .bss
	.word	g_d<--32bit symbol value:address in .data

*/
int main()
{
	return g_b + g_d;//to get the value of global var, its symbol value ie address must be got before referring its value
}
