.code16
.text
.globl _start
_start:
	jmp start
src:
	.byte 'L', 0x7, 'A', 0x7, 'B', 0x7, 'E', 0x7, 'L', 0x7, ' ', 0x7,\
	 'O', 0x7, 'F', 0x7, 'F', 0x7, 'S', 0x7, 'E', 0x7, 'T', 0x7, ':', 0x7
start:
	mov $0, %ax
	mov %ax, %ds
	mov $0xb800, %ax
	mov %ax, %es
	cld
	mov $src, %si
	mov $0, %di
	mov $((start - src) / 2), %cx
	rep movsw #ds:si=>es:di
	jmp .
obsolete:
	mov $-1, %ax
	mov $0xf, %dx
	//mov $0x1f, %dx//jump to:0) [0x0000000fff53] f000:ff53 (unk. ctxt): iret                      ; cf
	mov $0x10, %bx
	div %bx
	mov %dx, remainder
	mov %ax, quotient
	jmp .
remainder:
	.short 0
quotient:
	.short 0