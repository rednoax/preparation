.code16
.globl _start
_start:
    mov $0, %ax
    mov %ax, %ds
    mov $0xb800, %ax
    mov %ax, %es
    mov $0, %di
    mov $0, %si
    mov $0x7, %ah
    mov $len, %cx
prompt:
    mov str(%si), %al
    mov %ax, %es:(%di)
    inc %si
    add $2, %di
    loop prompt
#calc:
    mov $1, %ax
    mov $2, %cx
calc:
    add %cx, %ax
    inc %cx
    cmp $100, %cx #s: cx - 100 <= 0
    jle calc
    mov $4, %bx
divide:
    xor %dx, %dx
    divw divisor
    add $0x30, %dl
    mov $0x4, %dh
    mov %dx, %es:(%bx)
    dec %bx
    cmp $0, %ax #ax - 0
    je 1f
    jns divide
1:  jmp .
str:
    .ascii "1+2+3+...+100="
end:
     .set len, end - str
divisor:
    .byte 10
.number:
    .rep 5 
    .byte 0
    .endr