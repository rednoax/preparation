.code16
.globl _start #if no such: warning: cannot find entry symbol _start; defaulting to 0000000000000000
_start:
	mov $0xb800, %ax
	mov %ax, %es
#	mov $'x', (0x0);ch4.S:4: Error: no instruction mnemonic suffix given and no register operands; can't size instruction
	movb $'X', %es:(0x0)
	movb $'y', %es:(2)
	movb $'Z', %es:(4)
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
	jmp .