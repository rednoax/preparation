.code16
.globl _start
_start:
str: .ascii "1+2+3+...+100"
end: .set len, end - str
    mov $0, %ax
    mov %ax, %ds
    mov $0xb800, %ax
    mov %ax, %es
    mov $0, %di
    mov %ah, 0x7
    mov $len, %cx
prompt:
    mov str(%di), %al
    mov %ax, %es:(%di)
    inc %di
    loop prompt
    jmp .