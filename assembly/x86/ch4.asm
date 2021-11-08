;org 0x7c00 -removed as it is rejected when assembling with elf format.
bits 16
global _start ;if no such:warning: cannot find entry symbol _start; defaulting to 0000000000007c00
_start:
mov ax, label0
mov bx, label1 ;the above 2 are completely the same
;If the above 2 and the last 2 mov xx,[labelx] are put together, the later 2 instruction's fetch in qemu is not the 
;same as what `objdump -d -M i8086` shown. Then the latter 2 mov xx,[labelx] will not work. The workaround is to split
;the above 2 and the latter 2 mov xx,[labelx].

mov bx, 0xb800
mov ds, bx
mov byte [0x0], '2'
mov byte [0x1], 0x3
mov byte [0x2], 'S'
mov byte [0x3], 0x3
mov byte [0x4], 'M'
mov byte [0x5], 0x3

mov cx, 0
mov ds, cx
mov cx, [label0];cx:0xaa55
mov dx, [label1];dx:0x55aa. the above 2 are of the same function

;objdump -d -M i8086 ch4.asm.o proves the following 2 are equivalent
label0:db 0x55,0xaa
label1 db 0xaa,0x55

;there is 2 local symbols, relocatable/executable elf vma:0/7c00
; these 2 local symbols is referenced 4 times so there is 4 entries in .rel.text
;'readelf -a' & 'objdump -d -M i8086'
;1. relocatable elf
;Section Headers:
;  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
;  [ 1] .text             PROGBITS        00000000 0002c0 00003a 00  AX  0   0 16
;  [ 3] .symtab           SYMTAB          00000000 0003c0 000090 10      4   8  4
;  [ 5] .rel.text         REL             00000000 000470 000020 08      3   1  4<=type 'REL' means section is of relocation type.0x20/8==4 entries.Inf means which section to apply the relocation,1 is .text so [5]'s relocation entry's offset is relative to [1]. Lk means symbol table's index.

;Relocation section '.rel.text' at offset 0x470 contains 4 entries:
; Offset     Info    Type            Sym.Value  Sym. Name
;00000001  00000214 R_386_16          00000000   .text<=offset is BYTE OFFSET relative to 'Inf' section,see below _start assembly comments
;00000004  00000214 R_386_16          00000000   .text<=Info:[31:8]relocation table entry's symbol 's index in symbol table[7:0]:relocation entry type;
;00000030  00000214 R_386_16          00000000   .text<=type:0x14 seems R_386_16,0x2 means index 2 in .symtab, ie ".text", which is neither 'label0' nor 'label1' as we expect. REASON unknow. In theory the [31:8] indexed symbol's value ie address in exec-elf .symtab is the used to fill at the offset of .text.
;00000034  00000214 R_386_16          00000000   .text<=R_386_16 seems the fixed bytes is 16bits ie 2B.

;     2: 00000000     0 SECTION LOCAL  DEFAULT    1 
;     3: 00000036     1 OBJECT  LOCAL  DEFAULT    1 label0
;     4: 00000038     1 OBJECT  LOCAL  DEFAULT    1 label1
;00000000 <_start>:
;   0:   b8 36 00                mov    $0x36,%ax<=offset 1 in .rel.text means [36,... to be fixed;R_386_16 seems means fix 2B. In theory symbol 'label0'(actually the symbol is .text, not 'label0' as expected,reason unknow) in exec elf's value is 0x7c36, so at last 36 00 is fixed to be 36 7c
;   3:   bb 38 00                mov    $0x38,%bx<=offset 4 in .rel.text means [38,... to be fixed
;  2e:   8b 0e 36 00             mov    0x36,%cx<=offset 30 in .rel.text means [36,... to be fixed
;  32:   8b 16 38 00             mov    0x38,%dx<=offset 34 in .rel.text means [38,... to be fixed

;00000036 <label0>:
;  36:   55 aa                                               U.

;00000038 <label1>:
;  38:   aa 55                                               .U

;2. exec elf, no '.rel.text' section, only '.text' section
;    10: 00007c36     1 OBJECT  LOCAL  DEFAULT    1 label0
;    11: 00007c38     1 OBJECT  LOCAL  DEFAULT    1 label1

;00007c00 <_start>:
;    7c00:       b8 36 7c                mov    $0x7c36,%ax<= 36 7c <= 36 00
;    7c03:       bb 38 7c                mov    $0x7c38,%bx<= 38 7c <= 38 00
;    7c2e:       8b 0e 36 7c             mov    0x7c36,%cx<= 36 7c <= 36 00
;    7c32:       8b 16 38 7c             mov    0x7c38,%dx<= 38 7c <= 38 00

;00007c36 <label0>:
;    7c36:       55 aa                                               U.

;00007c38 <label1>:
;    7c38:       aa 55                                               .U


