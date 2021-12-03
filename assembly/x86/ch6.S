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
	rep movsw #ds:si=>es:di; one time 's' or 'n' can't finish this line
#rdi:0x1a, cx:0x0
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
	mov $0, %cx
	movw $0, number
	movw $0, number + 2 #<bochs:2> x 0x7c7e [bochs]: 0x0000000000007c7e <bogus+       0>:	0x00010000
delay:
	incw number
	jnz _loop
	incw number + 2
_loop:
	loop delay #cx count down range: 0, -1, ..., -0xffff: 0 then [0xffff, 1]
	#there is no counterpart for $$ in NASM
	movw $_start, divisor #objdump: 7c77:       c7 06 89 7c 00 7c       movw   $0x7c00,0x7c89
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
str1:
	.ascii "012"
str1_end:
	.equ len1, str1_end - str1 #3    21: 00000003     0 NOTYPE  LOCAL  DEFAULT  ABS len1
str2:
	.asciz "012"
str2_end:
	.set len2, str2_end - str2 #4    24: 00000004     0 NOTYPE  LOCAL  DEFAULT  ABS len2
