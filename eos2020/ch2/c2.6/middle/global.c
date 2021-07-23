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
/*
readelf -a:
  [22] .data             PROGBITS        00010508 000508 00000c 00  WA  0   0  4
  [23] .bss              NOBITS          00010514 000514 000008 00  WA  0   0  4
...
  78: 00010510     4 OBJECT  GLOBAL DEFAULT   22 g_d<-in .data
  89: 00010518     4 OBJECT  GLOBAL DEFAULT   23 g_b<-in .bss

objdump -s -D
Contents of section .data:
 10508 00000000 00000000 01000000           ............    
...
Disassembly of section .data:

00010508 <__data_start>:
   10508:	00000000 	.word	0x00000000

0001050c <__dso_handle>:
   1050c:	00000000 	.word	0x00000000

00010510 <g_d>:
   10510:	00000001 	.word	0x00000001 

.data	0x00010508
		0x0001050c
 g_d	0x00010510<--
.bss	0x00010514
 g_b	0x00010518<--

so g_d and g_b is just addresses, at .data and .bss respectively
*/
int main()
{
	return g_b + g_d;//to get the value of global var, its symbol value ie address must be got before referring its value
}
