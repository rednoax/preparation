.code16
.globl _start
#if 10
.section header
.align 16 #16-byte memory boundary aligned
header_end:
.section code_1
.align 16
_start:
._$:    #a symbol name can begin with any of '._$'
$._$:   #strage name, isn't it :)
    jmp .

.section code_2
.align 16

.section data_1
.align 16

.section data_2
.align 16

.section stack
.align 16
    .space 4096 #this directive emits 4K bytes, each of value 0
stack_end:
.section trail
.align 16
program_end:
/*
1. 'objdump -d' for execelf output none as no .text section in execelf. -Ttext can
  only affect .text.
x86$ objdump -D c08.out
c08.out:     file format elf32-i386

Disassembly of section code_1:
00000000 <_start>: <==vma is 0, not 7c00 even `-Ttext 0x7c00` during `make`
   0:   eb fe                   jmp    0 <_start>

Disassembly of section stack:
00000000 <stack_end-0x1000>:
        ...

Disassembly of section .debug_aranges:
...............
x86$ objdump -D c08.o|head -20

c08.o:     file format elf32-i386
Disassembly of section code_1: <==vma is 0,can't be 7c00 as `-Ttext 0x7c00` during `make` affect ld process only.
00000000 <_start>:
   0:   eb fe                   jmp    0 <_start>

Disassembly of section stack:
00000000 <stack_end-0x1000>:
        ...
Disassembly of section .debug_line:
.....
2. readelf -s c08.out
    ......
     9: 00000000     0 NOTYPE  LOCAL  DEFAULT    1 ._$
    10: 00000000     0 NOTYPE  LOCAL  DEFAULT    1 $._$
    ...
    12: 00000000     0 NOTYPE  GLOBAL DEFAULT    1 _start <=not 7c00
3. In the whole file only code_1 & stack contain real instruction or data:
x86$ readelf -S  c08.out
There are 11 section headers, starting at offset 0x12b0:

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] code_1            PROGBITS        00000000 000040 000002 00      0   0 16<=Addr is 0 rather than 0x7c00 as it's relocatable elf rather then execelf
  [ 2] stack             PROGBITS        00000000 000050 001000 00      0   0 16<=Addr is 0 too! So a link script is needed
  [ 3] .debug_aranges    PROGBITS        00000000 001050 000020 00      0   0  8
  .......
note stack is "PROGBITS" that means the gap filled with 0 is a part of section:
$ readelf -x stack c08.out
Hex dump of section 'stack':
  0x00000000 00000000 00000000 00000000 00000000 ................<==the other 0 are ignored
note there is empty size section in relocatable elf. But these empty section are all disappeared in exec-elf:
x86$ readelf -S  c08.o
There are 21 section headers, starting at offset 0x135c:

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .text             PROGBITS        00000000 000034 000000 00  AX  0   0  1<=0 size, addr is 0 rather than 0x7c00 as it's relocatable elf rather then execelf
  [ 2] .data             PROGBITS        00000000 000034 000000 00  WA  0   0  1<=0 size
  [ 3] .bss              NOBITS          00000000 000034 000000 00  WA  0   0  1<=0 size
  [ 4] header            PROGBITS        00000000 000040 000000 00      0   0 16<=0 size
  [ 5] code_1            PROGBITS        00000000 000040 000002 00      0   0 16<=2B size
  [ 6] code_2            PROGBITS        00000000 000050 000000 00      0   0 16<=0 size
  [ 7] data_1            PROGBITS        00000000 000050 000000 00      0   0 16<=0 size
  [ 8] data_2            PROGBITS        00000000 000050 000000 00      0   0 16<=0 size
  [ 9] stack             PROGBITS        00000000 000050 001000 00      0   0 16<=0x1000B size
  [10] .debug_line       PROGBITS        00000000 001050 000035 00      0   0  1
  ..............
*/
#else
_start:
.org 510 #from https://www.cs.usfca.edu/%7Ecruse/cs630f06/bootmsw.s
    .word 0xaa55 #the gap will be filled with 0, `readelf -S` show the section size==0x200 B
#00007c00 <_start>:<==execelf objdump
#    7c00:       eb fe                   jmp    7c00 <_start>
#        ...
#    7dfe:       55                      push   %bp
#    7dff:       aa                      stos   %al,%es:(%di)
#endif