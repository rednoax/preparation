#$ make
#$ ./vhd.sh cp5.3.out.bin
#start virtualbox in GUI
.code16
.text
.globl _start
_start:
	mov $0xb800, %ax
	mov %ax, %ds
	mov $0, %ax
	mov %ax, %es
	movb $'L', (0x0)
	movb $'A', (0x2)
	movb $'B', (0x4)
	movw $0x2008, data1
	xorb $0x5, data1
	addw $0x0101, data1
	mov data1, %ax
div1:
	mov $0xa000, %ax
	mov $0x9000, %bx
	mov $1, %dx
	div %bx # (dx<<16+ax)/bx:dx(yushu):0x8000 ax(shang):0x2 bx(not changed):9000
	mov $0xa000, %ax
	mov $0x9000, %bx
	mov $1, %dx
	div %bh #in theory: ah(yushu): 0x40 al(shang):0x11c, but qemu will be in loop when running this instruction. reason uknown.
	jmp .
l0: #the label in 'jmp' is not interpreted as label in 'mov'
#	jmp $l0 #Error: operand type mismatch for `jmp'
#	jmp l0 #ok
data1:
	.word 0

#[mem]:in nasm format:[bx/bp+si/di+imm8/16], no need for all 3 existing, any 1 or any 2 is ok.
#[imm8/16] can be [label], which is `label` in gnu AT&T format eg:
#NASM: mov ax,[label];GNU equivalent: mov label, %ax
#label in exec-elf is address, which will be a part of hex code of 'mov'

#mov/xor/add:
#src operand:8/16bit general register or imm8/16 or [mem]
#dest operand:the same as src operand but can't be imm8/16
#src&dest operand can not be [mem] at the same time, and 2 operands' size must be equal(note 'mov $0xa, %ax' is ok!)
#
#div:divisor:8/16bit general register or [mem], can't be imm8/16

#the NASM equivalent is in the right
	mov data1, %ax #mov ax, [data1]
#	div data1#Error: no instruction mnemonic suffix given and no register operands; can't size instruction
	divw data1 #div word [data1]
	xor %dx, %ax #xor ax, dx
	divb data1 #div byte [data1]<==f6 36 2f 7c divb   0x7c2f, 00007c2f <data1>: label's value ie address becomes a part of instruction
	xor data1, %al #xor al, [data1]
# the following is ok! Strange!
	add $0x5, data1 #add [data1],0x5
#	xor 0x55, 0xff #Error: too many memory references for `xor'<==there can't be 2 mem references! NASM equivalent:xor [0xff],[0x55]
#	xor $0x55, $0xff #Error: operand type mismatch for `xor', NASM equivalent: xor 0xff, 0x55
#	add %al, $0x6 #Error: operand type mismatch for `add'<=dest operand is imme
#	div 0xf0 #Error: no instruction mnemonic suffix given and no register operands; can't size instruction
#	div $0xf0 #NASM equivalent: div 0xf0 #Error: operand type mismatch for `div'; div operand can't be imm
#	add %cl, %ax #Error: operand type mismatch for `add', cl is 8bit and ax is 16bit

