//#pragma optimize level=0
	.code16 /*qemu-i386 label.out: emu: uncaught target signal 11 (Segmentation fault) - core dumped
Segmentation fault (core dumped); if .code16 is changed to .code32, each ins will be prefixed with 0x66
and `qemu-i386 label.out` runs normally*/
.text
	.global _start
_start:
	mov $0x3f, %ax
	mov $0x1, %bx
	add %ax, %bx
#	mov $0, %ds
	jmp .
	.code32 //each instruction below has a 0x66 prefix
	mov $0x3f, %ax
	mov $0x1, %bx
	add %ax, %bx