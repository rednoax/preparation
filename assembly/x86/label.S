//#pragma optimize level=0
	.code16
.text
	.global _start
_start:
	mov $0x3f, %ax
	mov $0x1, %bx
	add %ax, %bx
	jmp .