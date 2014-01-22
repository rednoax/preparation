#include <stdio.h>
#if 0
6.2 Locally Declared Labels
GCC allows you to declare local labels in any nested block scope. A local label is just like
an ordinary label, but you can only reference it (with a goto statement, or by taking its
address) within the block in which it was declared.
A local label declaration looks like this:
__label__ label;
or
__label__ label1, label2, /* . . . */;
Local label declarations must come at the beginning of the block, before any ordinary
declarations or statements.
The label declaration defines the label name, but does not define the label itself. You mus
do this in the usual way, with label:, within the statements of the statement expression.
The local label feature is useful for complex macros. If a macro contains nested loops, a
goto can be useful for breaking out of them. However, an ordinary label whose scope is th
whole function cannot be used: if the macro can be expanded several times in one function
the label will be multiply defined in that function. A local label avoids this problem. Fo
example:
......
LS: && is unary(yiyuande) operator to get the address of a label
#endif
#define _THIS_IP_ ({__label__ __here;__here:(unsigned long)&&__here;})
#if 0
6.3 Labels as Values
You can get the address of a label defined in the current function (or a containing function)
with the unary operator ¡®&&¡¯. The value has type void *. This value is a constant and can
be used wherever a constant of that type is valid. For example:
void *ptr;
/* . . . */
ptr = &&foo;
#endif
#define __this_ip__ ({__here:(unsigned long)&&__here;})
int main()
{
	/*
	one usual way to get a symbol, one global symbol with the same name as the local label
	is ok!
	*/
	extern unsigned long __here;
	asm volatile(".globl __here;__here:");
#if 0
	/*
	extern unsigned long __here[];<---changed type
Disassembly of section .text:

000080f0 <main>:
    80f0:       e92d4008        push    {r3, lr}

000080f4 <__here>:
    80f4:       e59f1040        ldr     r1, [pc, #64]   ; 813c <__here+0x48><---__here is changed to address once its declaration is changed to pointer
    80f8:       e59f0040        ldr     r0, [pc, #64]   ; 8140 <__here+0x4c>
    80fc:       eb00007d        bl      82f8 <__GI_printf>	
    8100:       e59f203c        ldr     r2, [pc, #60]   ; 8144 <__here+0x50>
    8104:       e59f103c        ldr     r1, [pc, #60]   ; 8148 <__here+0x54>
    8108:       e59f003c        ldr     r0, [pc, #60]   ; 814c <__here+0x58>
    810c:       eb000079        bl      82f8 <__GI_printf>
    8110:       e59f1038        ldr     r1, [pc, #56]   ; 8150 <__here+0x5c>
    8114:       e59f0038        ldr     r0, [pc, #56]   ; 8154 <__here+0x60>
    8118:       eb000076        bl      82f8 <__GI_printf>
    811c:       e59f1034        ldr     r1, [pc, #52]   ; 8158 <__here+0x64>
    8120:       e59f0034        ldr     r0, [pc, #52]   ; 815c <__here+0x68>
    8124:       eb000073        bl      82f8 <__GI_printf>
    8128:       e59f1030        ldr     r1, [pc, #48]   ; 8160 <__here+0x6c>
    812c:       e59f0030        ldr     r0, [pc, #48]   ; 8164 <__here+0x70>
    8130:       eb000070        bl      82f8 <__GI_printf>
    8134:       e3a00000        mov     r0, #0
    8138:       e8bd8008        pop     {r3, pc}
    813c:       000080f4        strdeq  r8, [r0], -r4<---r1 is address of __here
    8140:       0000e410        andeq   lr, r0, r0, lsl r4
    8144:       000080f0        strdeq  r8, [r0], -r0
    8148:       000080f0        strdeq  r8, [r0], -r0
    814c:       0000e42a        andeq   lr, r0, sl, lsr #8
    8150:       000080f0        strdeq  r8, [r0], -r0
    8154:       0000e43c        andeq   lr, r0, ip, lsr r4
    8158:       000080f0        strdeq  r8, [r0], -r0
    815c:       0000e451        andeq   lr, r0, r1, asr r4
    8160:       000080f0        strdeq  r8, [r0], -r0
    8164:       0000e474        andeq   lr, r0, r4, ror r4

00008168 <_start>:
    8168:       e3a0b000        mov     fp, #0	
	*/
#endif	
	printf("global symbol __here:%lx\n", __here);
	/*
	__asm__ __volatile__("__here:");
	printf("local symbol __here:%p\n", &&__here);
	will emit error:
_THIS_IP_.c: Assembler messages:
_THIS_IP_.c:43: Error: symbol `__here' is already defined
	*/
	//there is no __label__ declaration for __here2 and HERE, so they are visible in whole func
	__here2:
	HERE:
	printf("_THIS_IP_ %p, %p\n", &&__here2, &&HERE);
	printf("1st local label:%lx\n", __this_ip__);
	/*
	use __this_ip__ 2nd time like "printf("another local label:%lx\n", __this_ip__);
	" will emit error:
[root@localhost preparation]# gcc -Wall _THIS_IP_.c
_THIS_IP_.c: In function 'main':
_THIS_IP_.c:4:23: error: duplicate label '__here'
 #define __this_ip__ ({__here:(unsigned long)&&__here;})
                       ^
_THIS_IP_.c:14:38: note: in expansion of macro '__this_ip__'
  printf("another local label:%lx\n", __this_ip__);
                                      ^
_THIS_IP_.c:4:23: note: previous definition of '__here' was here
 #define __this_ip__ ({__here:(unsigned long)&&__here;})
                       ^
_THIS_IP_.c:13:34: note: in expansion of macro '__this_ip__'
  printf("1st local label:%lx\n", __this_ip__);
                                  ^	
	*/
	//but the following is ok since __label__ declaration make the label only exists in ({}) block!
	printf("really 1st local label __here:%lx\n", _THIS_IP_);
#if 0//-Os -S
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
.L2:
.L3:
.L4:
.L5:
.L6:
	stmfd	sp!, {r3, lr}
#APP
@ 41 "_THIS_IP_.c" 1
	.globl __here;__here:
@ 0 "" 2
	ldr	r3, .L7
	ldr	r0, .L7+4
	ldr	r1, [r3, #0]
	bl	printf
	ldr	r2, .L7+8
	ldr	r1, .L7+12
	ldr	r0, .L7+16
	bl	printf
	ldr	r1, .L7+20
	ldr	r0, .L7+24
	bl	printf
	ldr	r1, .L7+28
	ldr	r0, .L7+32
	bl	printf
	ldr	r1, .L7+36<---r1=.L6
	ldr	r0, .L7+40<---r0=.LC8
	bl	printf<---the last printf
	mov	r0, #0
	ldmfd	sp!, {r3, pc}
.L8:
	.align	2
.L7:
	.word	__here
	.word	.LC0
	.word	.L3
	.word	.L2
	.word	.LC1
	.word	.L4
	.word	.LC4
	.word	.L5
	.word	.LC6
	.word	.L6
	.word	.LC8
	.size	main, .-main
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"global symbol __here:%lx\012\000"
.LC1:
	.ascii	"_THIS_IP_ %p, %p\012\000"
.LC4:
	.ascii	"1st local label:%lx\012\000"
.LC6:
	.ascii	"really 1st local label __here:%lx\012\000"
.LC8:
	.ascii	"really 2nd local label __here:%lx\012\000"
	.ident	"GCC: (Buildroot 2013.05) 4.7.3"
	.section	.note.GNU-stack,"",%progbits
________
//-Os -static 
a.out:     file format elf32-littlearm


Disassembly of section .init:

000080d4 <_init>:
    80d4:	e1a0c00d 	mov	ip, sp
    80d8:	e92ddff0 	push	{r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}
    80dc:	e24cb004 	sub	fp, ip, #4
    80e0:	e91baff0 	ldmdb	fp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}

Disassembly of section .text:

000080f0 <main>:
    80f0:	e92d4008 	push	{r3, lr}

000080f4 <__here>:
    80f4:	e59f3044 	ldr	r3, [pc, #68]	; 8140 <__here+0x4c><---label address 80f4
    80f8:	e59f0044 	ldr	r0, [pc, #68]	; 8144 <__here+0x50>
    80fc:	e5931000 	ldr	r1, [r3]<---r1 = e59f3044
    8100:	eb00007d 	bl	82fc <__GI_printf>
    8104:	e59f203c 	ldr	r2, [pc, #60]	; 8148 <__here+0x54>
    8108:	e59f103c 	ldr	r1, [pc, #60]	; 814c <__here+0x58><---label address 80f0
    810c:	e59f003c 	ldr	r0, [pc, #60]	; 8150 <__here+0x5c><---r0 == e42a, contain fmt char*
    8110:	eb000079 	bl	82fc <__GI_printf>
    8114:	e59f1038 	ldr	r1, [pc, #56]	; 8154 <__here+0x60>
    8118:	e59f0038 	ldr	r0, [pc, #56]	; 8158 <__here+0x64>
    811c:	eb000076 	bl	82fc <__GI_printf>
    8120:	e59f1034 	ldr	r1, [pc, #52]	; 815c <__here+0x68>
    8124:	e59f0034 	ldr	r0, [pc, #52]	; 8160 <__here+0x6c>
    8128:	eb000073 	bl	82fc <__GI_printf>
    812c:	e59f1030 	ldr	r1, [pc, #48]	; 8164 <__here+0x70><---label address 80f0
    8130:	e59f0030 	ldr	r0, [pc, #48]	; 8168 <__here+0x74>
    8134:	eb000070 	bl	82fc <__GI_printf>
    8138:	e3a00000 	mov	r0, #0
    813c:	e8bd8008 	pop	{r3, pc}
    8140:	000080f4 	strdeq	r8, [r0], -r4
    8144:	0000e410 	andeq	lr, r0, r0, lsl r4
    8148:	000080f0 	strdeq	r8, [r0], -r0
    814c:	000080f0 	strdeq	r8, [r0], -r0
    8150:	0000e42a 	andeq	lr, r0, sl, lsr #8
    8154:	000080f0 	strdeq	r8, [r0], -r0
    8158:	0000e43c 	andeq	lr, r0, ip, lsr r4
    815c:	000080f0 	strdeq	r8, [r0], -r0
    8160:	0000e451 	andeq	lr, r0, r1, asr r4
    8164:	000080f0 	strdeq	r8, [r0], -r0
    8168:	0000e474 	andeq	lr, r0, r4, ror r4
......
Disassembly of section .fini:

0000e400 <_fini>:
    e400:	e1a0c00d 	mov	ip, sp
    e404:	e92ddff0 	push	{r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}
    e408:	e24cb004 	sub	fp, ip, #4
    e40c:	e91baff0 	ldmdb	fp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}

Disassembly of section .rodata:

0000e410 <spec_flags.8387-0x10a>:
    e410:	626f6c67 	rsbvs	r6, pc, #26368	; 0x6700
    e414:	73206c61 	teqvc	r0, #24832	; 0x6100
    e418:	6f626d79 	svcvs	0x00626d79
    e41c:	5f5f206c 	svcpl	0x005f206c
    e420:	65726568 	ldrbvs	r6, [r2, #-1384]!	; 0x568
    e424:	786c253a 	stmdavc	ip!, {r1, r3, r4, r5, r8, sl, sp}^
    e428:	545f000a 	ldrbpl	r0, [pc], #-10	; e430 <_fini+0x30>
    e42c:	5f534948 	svcpl	0x00534948
    e430:	205f5049 	subscs	r5, pc, r9, asr #32
    e434:	202c7025 	eorcs	r7, ip, r5, lsr #32
    e438:	000a7025 	andeq	r7, sl, r5, lsr #32
    e43c:	20747331 	rsbscs	r7, r4, r1, lsr r3
    e440:	61636f6c 	cmnvs	r3, ip, ror #30
    e444:	616c206c 	cmnvs	ip, ip, rrx
    e448:	3a6c6562 	bcc	1b279d8 <__bss_end__+0x1b0dc28>
    e44c:	0a786c25 	beq	1e294e8 <__bss_end__+0x1e0f738>
    e450:	61657200 	cmnvs	r5, r0, lsl #4
    e454:	20796c6c 	rsbscs	r6, r9, ip, ror #24
    e458:	20747331 	rsbscs	r7, r4, r1, lsr r3
    e45c:	61636f6c 	cmnvs	r3, ip, ror #30
    e460:	616c206c 	cmnvs	ip, ip, rrx
    e464:	206c6562 	rsbcs	r6, ip, r2, ror #10
    e468:	65685f5f 	strbvs	r5, [r8, #-3935]!	; 0xf5f
    e46c:	253a6572 	ldrcs	r6, [sl, #-1394]!	; 0x572
    e470:	000a786c 	andeq	r7, sl, ip, ror #16
    e474:	6c616572 	cfstr64vs	mvdx6, [r1], #-456	; 0xfffffe38
    e478:	3220796c 	eorcc	r7, r0, #1769472	; 0x1b0000
    e47c:	6c20646e 	cfstrsvs	mvf6, [r0], #-440	; 0xfffffe48
    e480:	6c61636f 	stclvs	3, cr6, [r1], #-444	; 0xfffffe44
    e484:	62616c20 	rsbvs	r6, r1, #8192	; 0x2000
    e488:	5f206c65 	svcpl	0x00206c65
    e48c:	7265685f 	rsbvc	r6, r5, #6225920	; 0x5f0000
    e490:	6c253a65 	stcvs	10, cr3, [r5], #-404	; 0xfffffe6c
    e494:	10000a78 	andne	r0, r0, r8, ror sl
    e498:	0a081010 	beq	2124e0 <__bss_end__+0x1f8730>
    e49c:	002b0a0a 	eoreq	r0, fp, sl, lsl #20
    e4a0:	0020002d 	eoreq	r0, r0, sp, lsr #32
    e4a4:	30007830 	andcc	r7, r0, r0, lsr r8
    e4a8:	6e280058 	mcrvs	0, 1, r0, cr8, cr8, {2}
    e4ac:	00296c69 	eoreq	r6, r9, r9, ror #24
    e4b0:	6c756e28 	ldclvs	14, cr6, [r5], #-160	; 0xffffff60
    e4b4:	0000296c 	andeq	r2, r0, ip, ror #18
    e4b8:	6a4c6c68 	bvs	1329660 <__bss_end__+0x130f8b0>
    e4bc:	5a71747a 	bpl	1c6b6ac <__bss_end__+0x1c518fc>
    e4c0:	08040200 	stmdaeq	r4, {r9}
    e4c4:	08000008 	stmdaeq	r0, {r3}
    e4c8:	08010000 	stmdaeq	r1, {}	; <UNPREDICTABLE>
    e4cc:	0f070100 	svceq	0x00070100
    e4d0:	13121110 	tstne	r2, #4
    e4d4:	00080015 	andeq	r0, r8, r5, lsl r0
    e4d8:	00030005 	andeq	r0, r3, r5
    e4dc:	00010004 	andeq	r0, r1, r4
    e4e0:	00000200 	andeq	r0, r0, r0, lsl #4
    e4e4:	08000400 	stmdaeq	r0, {sl}
    e4e8:	00070002 	andeq	r0, r7, r2
    e4ec:	10000807 	andne	r0, r0, r7, lsl #16
    e4f0:	00000005 	andeq	r0, r0, r5
    e4f4:	00020007 	andeq	r0, r2, r7
    e4f8:	00010004 	andeq	r0, r1, r4
    e4fc:	1f000003 	svcne	0x00000003
    e500:	0f000005 	svceq	0x00000005
    e504:	00020807 	andeq	r0, r2, r7, lsl #16
    e508:	00010004 	andeq	r0, r1, r4
    e50c:	01000003 	tsteq	r0, r3
    e510:	02040101 	andeq	r0, r4, #1073741824	; 0x40000000
    e514:	01010101 	tsteq	r1, r1, lsl #2
    e518:	2b200101 	blcs	80e924 <__bss_end__+0x7f4b74>
#endif
	printf("really 2nd local label __here:%lx\n", _THIS_IP_);
	return 0;
}