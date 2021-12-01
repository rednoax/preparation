#define NR (divisor - number)
.code16
.text
.globl _start
_start:
	jmp start
src:
	.byte 'L', 0x7, 'A', 0x7, 'B', 0x7, 'E', 0x7, 'L', 0x7, ' ', 0x7,\
	 'O', 0x7, 'F', 0x7, 'F', 0x7, 'S', 0x7, 'E', 0x7, 'T', 0x7, ':', 0x7 #0x1a bytes
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
#rdi:0x1a
	mov $number, %ax #todo:if number>0xffff?
	mov $0, %si
func:
	xor %dx, %dx #mov $0, %dx
	divw divisor
	movb %dl, number(%si) #AT&T: imm(base,index) or imm(index)
	inc %si #can be rm8/16 but not immedite
	cmp $NR, %si #si - 5
	jl func # signed <
#	mov $0, %di
	dec %si #si 4<=5
display:
	mov number(%si), %bl
	add $0x30, %bl
	mov $0x4, %bh
	mov %bx, %es:(%di) #movb $0x4, %es:1(%di)
	add $2, %di
	dec %si
#if 0
	cmp $0, %si #si - 0
	jge display #signed >=
#else
	jns display
#endif
	jmp .
	mov (%bp), %ax #mov ax, word ptr ss:[bp]  ; 8b4600
	mov (%bp,%si), %ax #mov ax, word ptr ss:[bp+si] ; 8b02
number:
	.byte 0, 0, 0, 0, 0
divisor:
	.byte 10, 0
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