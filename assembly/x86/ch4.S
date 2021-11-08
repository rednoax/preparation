.code16
.globl _start #if no such: warning: cannot find entry symbol _start; defaulting to 0000000000000000
_start:
	mov $0xb800, %ax
	mov %ax, %es
#	mov $'x', (0x0);ch4.S:4: Error: no instruction mnemonic suffix given and no register operands; can't size instruction
	movb $'0', %es:(0x0)
	movb $'x', %es:(2)
	movb $'0', %es:(4)
#mov %bl, %ax #ch4.S:10: Error: operand size mismatch for `mov'
#mov $0x55aa, %al #ch4.S:11: Warning: 21930(0x55aa) shortened to 170(0xaa)
#mov $0x6000, %ds #ch4.S:12: Error: operand type mismatch for `mov'
#mov %al, %ds #ch4.S:13: Error: operand type mismatch for `mov'
#mov $0x55aa, (6) #ch4.S:14: Error: no instruction mnemonic suffix given and no register operands; can't size instruction
	mov %bx, %ds
	mov $2, %ax
	mov %ax, (0xa)
	mov %cx, %es
#mov %bl, %ax #Error: operand size mismatch for `mov'
	movb $'c', (0x0)
#movw (0xf000), (2) #Error: too many memory references for `mov'
#mov $3, %ds#Error: operand type mismatch for `mov'
	mov $0, %ax
	mov %ax, %ds
	mov $label0, %ax//label0 symbol value is 7c2e in .symtab, ax==0x7c2e
	mov label0, %ax //a1 2e 7c  mov  0x7c2e,%ax; 00007c2e <label0>; instruction contains label0's abs address 0x7c2e. ax=[ds<<4+0x7c2], then ax==0xaa55; the above 2 instruction's label0's symbol value are all part of instructions:
#    7c2b:       b8 33 7c                mov    $0x7c33,%ax
#    7c2e:       a1 33 7c                mov    0x7c33,%ax

	jmp .

label0:
	.byte 0x55,0xaa
