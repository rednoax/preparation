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
    mov $1, %cx
    mov $0, %ax
1:
    add %cx, %ax
    inc %cx
    cmp $100, %cx #s: cx - 100 <= 0
    jle 1b
#if 0
    mov $8, %bx
divide:
    xor %dx, %dx
    divw divisor
    add $0x30, %dl
    mov $0x4, %dh
    mov %dx, %es:(%bx, %di)//(%di,%bx):`%es:(%di,%bx)' is not a valid base/index expression
    sub $2, %bx
    cmp $0, %ax #ax - 0
    jne divide
#else
    mov $0, %dx
    mov %dx, %ss
    mov $0xffff, %sp
    mov %sp, %bp
1:  mov $0, %dx
    divw divisor
    add $0x30, %dl
    mov $0x4, %dh
    push %dx
    cmp $0, %ax
    jne 1b
1:
    pop %dx
    mov %dx, %es:(%di)
    add $2, %di
    cmp %sp, %bp #u: bp - sp >0
    ja 1b
#endif
    jmp .
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