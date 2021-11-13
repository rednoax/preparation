.code16
.globl _start
_start:
	mov $0xb800, %ax
	mov %ax, %es
	mov $0, %bx
	mov %bx, %ds
	movb $'L', %es:(%bx)
	movb $0x7, %es:1(%bx)
	movb $'a', %es:2(%bx)
	movb $0x7, %es:3(%bx)
	movb $'b', %es:(0x4)
	movb $0x7, %es:(0x5)
	movb $'e', %es:(0x6)
	movb $0x7, %es:(0x7)
	movb $'l', %es:(0x8)
	movb $0x7, %es:(0x9)
	movb $' ', %es:(0xa)
	movb $0x7, %es:(0xb)
	movb $'o', %es:(0xc)
	movb $0x7, %es:(0xd)
	movb $'f', %es:(0xe)
	movb $0x7, %es:(0xf)
	movb $'f', %es:(0x10)
	movb $0x7, %es:(0x11)
	movb $'s', %es:(0x12)
	movb $0x7, %es:(0x13)
	movb $'e', %es:(0x14)
	movb $0x7, %es:(0x15)
	movb $'t', %es:(0x16)
	movb $0x7, %es:(0x17)
	movb $':', %es:(0x18)
	movb $0x7, %es:(0x19) #

	xor %dx, %dx
	mov $10, %bx
	mov $number, %ax
	div %bx #remainder: dx qutient: ax
	mov %dl, number + 4
	xor %dx, %dx
	div %bx
	mov %dl, number + 3
	xor %dx, %dx
	div %bx
	mov %dl, number + 2
	xor %dx, %dx
	div %bx
	mov %dl, number + 1
	xor %dx, %dx
	div %bx
	mov %dl, number + 0

	mov number, %al
	add $0x30, %al
	mov %al, %es:(0x20)
	mov number + 1, %al
	add $0x30, %al
	mov %al, %es:(0x22)
	mov number + 2, %al
	add $0x30, %al
	mov %al, %es:(0x24)
	mov number + 3, %al
	add $0x30, %al
	mov %al, %es:(0x26)
	mov number + 4, %al
	add $0x30, %al
	mov %al, %es:(0x28)
	movb $'D', %es:(0x2a)
	jmp .
number:
	.byte 0, 0, 0, 0, 0 #[0x7c00, 0x7e00), 0x7e00==32256