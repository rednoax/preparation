#[0cx800,...]
.code16
.globl program_end, start, header_end
#if 0
.section header#ld: c08.out: warning: allocated section `.text.head' not in segment
#else
.section header, "aw"
#endif
.align 16
program_length:
    .int program_end
code_entry:
    .word start #+4, eg 0x0022
    .int section.code_1.start #+6, 0x0020
realloc_tbl_len:
    .word (header_end-code_1_segment)/4
code_1_segment:
    .int section.code_1.start
code_2_segment:
    .int section.code_2.start
data_1_segment:
    .int section.data_1.start
data_2_segment:
    .int section.data_2.start
stack_segment:
    .int section.stack.start
header_end:

.section code_1
.align 16
put_string: #DS:BX asciz string
    push %bx
    push %cx
next:
    mov (%bx), %cl
    cmp $0, %cl
    jz fin
    call put_char
    inc %bx
    jmp next
fin:
    pop %bx
    pop %cx
    ret

put_char: #cl contains the char
    push %ax
    push %bx
    push %cx
    push %si
    push %di
    call get_cursor #ax cursor 16 bits
    cmp $'\r', %cl #0xd: carriage return
    jnz 0f
    divb divisor #ax/80, max(ax):2000
    mov $0, %ah
    mulb divisor #ax=al*80
    jmp 1f
0:
    cmp $'\n', %cl #0xa: line feed
    jnz normal
    add $80, %ax #ax += 80
    jmp 1f
normal:
    push %ds
    mov $0xb800, %bx
    mov %bx, %ds
    mov %ax, %bx #ax is 1999 at most
    shl $1, %bx
    mov %cl, (%bx)
    add $1, %ax
    pop %ds
1:
    cmp $25*80, %ax #u ax - 2000 < 0, hex(2000):0x7d0
    jnae 2f
movupClear: #ds/es:0x1000
    mov $24*80, %bx
    mov $80, %si
    mov $0, %di
    mov %bx, %cx
    cld
    rep movsw
    mov $80, %cx
.5: #1.5:#Error: junk at end of line, first unrecognized character is `1'
    movw $0x0720, (%bx)
    add $2, %bx
    loop .5
2:
    call set_cursor
    pop %di
    pop %si
    pop %cx
    pop %bx
    pop %ax
    ret
get_cursor: #out ax:[0,1999]
    push %dx
    mov $0x3d4, %dx
    mov $0xe, %al   #8bit port: cursor higher 8 bit
    out %al, %dx
    mov $0x3d5, %dx
    in %dx, %al
    mov %al, %ah
    mov $0x3d4, %dx
    mov $0xf, %al   #8bit port: cursor lower 8 bit
    out %al, %dx
    mov $0x3d5, %dx
    in %dx, %al
    pop %dx
    ret
set_cursor: #ax [15:8] cursor high 8bits [7:0] low 8bits
    push %bx
    push %dx
    mov %ax, %bx
    mov $0x3d4, %dx
    mov $0xe, %al #0xe: cursor high 8 bit
    out %al, %dx
    mov $0x3d5, %dx
    mov %ah, %al
    out %al, %dx #
    mov $0x3d4, %dx
    mov $0xf, %al #0xf: cursor low 8 bit
    out %al, %dx
    mov $0x3d5, %dx
    mov %bx, %ax
    out %al, %dx
    pop %dx
    pop %bx
    ret
start: #cs,ds,es:0x1000 already, but set them once more
    mov %cs, %ax
    mov %ax, %ds
    mov %ax, %es #mov %cs, %es;Error: operand type mismatch for `mov'
    mov %ax, %ss
    mov $stack_end, %sp
#if 0
    mov $msg0, %bx
    call put_string
#endif
    mov $msg1, %bx
    call put_string
    jmp .
divisor: .byte 80
.section code_2
.align 16
begin:

.section data_1
.align 16
msg0:
    .ascii "  This is NASM - the famous Netwide Assembler. \r\n"\
    "Back at SourceForge and in intensive development! \r\n"\
    "Get the current versions from http://www.nasm.us/.\r\n"\
    "\r\n\r\n"\
    "  Example code for calculate 1+2+...+1000: \r\n\r\n"\
    "     xor dx,dx\r\n"\
    "     xor ax,ax\r\n"\
    "     xor cx,cx\r\n"\
    "  @@:\r\n"\
    "     inc cx\r\n"\
    "     add ax,cx\r\n"\
    "     adc dx,0\r\n"\
    "     inc cx\r\n"\
    "     cmp cx,1000\r\n"\
    "     jle @@\r\n"\
    "     ... ...(Some other codes)\r\n\r\n\0"
.section data_2
.align 16
msg1:
    .ascii "  The above contents is written by LeeChung. \r\n"\
         "2011-05-06\0"
#.asciz "string1" "string2"#readelf -x .data proves each splitted string has its own \0 so: string1\0 string2\0
//program_end:
#define NOBITS
#ifdef NOBITS
.section stack, "aw", %nobits
#else
.section stack#, "aw"
#endif
.align 16
    .space 4096
stack_end:
.section trail, "aw", %nobits
.align 16
program_end:
/*
for c8.o relocatable object:
NOBITS defined: make c08.o not contain section 'stack',whose type is NOBITS:
  [10] stack             NOBITS          00000000 000220 001000 00  WA  0   0 16
and c08.o is smaller then NOBITS undefined;
rw-rw-r-- 1 rednoah rednoah 2464 Dec  4 18:26 c08.o
NOBITS undefined: make c08.o contain section 'stack', whose type os PROGBITS:
  [10] stack             PROGBITS        00000000 000220 001000 00      0   0 16
-rw-rw-r-- 1 rednoah rednoah  6560 Dec  4 18:28 c08.o
*/
/*
The section 1,2,3's Addr is continuous due to commands in .lds
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .text.head        PROGBITS        00000000 001000 000020 00  AX  0   0 16
  [ 2] .data             PROGBITS        00000020 001020 0001ba 00  WA  0   0 16
  [ 3] stack             NOBITS          000001e0 0011da 001000 00  WA  0   0 16
Comparing with ch8.S building that has no .lds take part in, whose beginning Addr
of each section is from 0, not continous.
*/