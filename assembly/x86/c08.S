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
    .word start #+4
    .int section.code_1.start #+6
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
start:
    jmp .

.section code_2
.align 16
begin:

.section data_1
.align 16
msg0:
    .asciz "  This is NASM - the famous Netwide Assembler. \r\n"\
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
    "     ... ...(Some other codes)\r\n\r\n"
.section data_2
.align 16
msg1:
    .asciz "  The above contents is written by LeeChung. \r\n"\
         "2011-05-06"
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