.text
.global _start
_start:
//	ldr r0, fb //Error: internal_relocation (type: OFFSET_IMM) not fixed up
#if 0
	ldr r0, =fb//ok, will be translated to the block of the following branch
#else
	ldr r0, label //gnu assembly's label is equals to [mem] in x86 NASM format
label:
	.word fb
#endif
	nop
	ldr r0, [r0]
	ldr sp, =stack_end
	push {r0}
	pop {r0}
stack:
	.space 32
stack_end:
.data
.global fb
fb:
	.word 0x55aa55aa
/*
Disassembly of section .text:

00008000 <_start>:
    8000:       e59f0000        ldr     r0, [pc]        ; 8008 <label>
    8004:       e5900000        ldr     r0, [r0]

00008008 <label>:
    8008:       0001800c        andeq   r8, r1, ip<=1800c is address of symbol 'fb', at last 1800c becomes a part of .text assembly;in 8086, it will becomes a part of instruction eg: mov data1, %ax: a1 2f 7c mov    0x7c2f,%ax<==7c2f <data1>:

Disassembly of section .data:

0001800c <__data_start>:
   1800c:       55aa55aa        strpl   r5, [sl, #1450]!        ; 0x5aa
===
equivalent in Gnu x86 and arm assembly
x86/arm:
mov $label, %ax;	ldr r0, =label 	<=all: reg=symbol table's symbol value; there are prefix before label on 2 platforms:$ for x86 and = for arm
mov label, %ax;		ldr r0, label	<=all: reg=[mem], mem is label's value in symbol table; there are no any prefix before label on 2 platforms
*/