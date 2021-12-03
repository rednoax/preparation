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
    jmp .
str: .ascii "1+2+3+...+100="
end: .set len, end - str
