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
prompt:
    mov %al, str(%di)
    mov %ax, %es:(%di)
    inc %di
    cmp $len, %di #u: %di - $len < 0
    jl prompt
    jmp .