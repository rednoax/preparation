;org 0x7c00 -removed as it is rejected when assembling with elf format.
bits 16
global _start ;if no such:warning: cannot find entry symbol _start; defaulting to 0000000000007c00
_start:
mov bx, 0xb800
mov ds, bx
mov byte [0x0], 'A'
mov byte [0x1], 0x3
mov byte [0x2], 'S'
mov byte [0x3], 0x3
mov byte [0x4], 'M'
mov byte [0x5], 0x3

mov ax, label0
mov bx, label1 ;the above 2 are completely the same
mov cx, [label0]
mov dx, [label1];the above 2 are completely the same

;objdump -d -M i8086 ch4.asm.o proves the following 2 are equivalent
label0:db 1,1
label1 db 1,1
