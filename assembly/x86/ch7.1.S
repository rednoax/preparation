.code16
.globl _start
_start:
    mov $0xb800, %bx
    mov %bx, %es
    mov $0, %bx
    mov %bx, %ss
    mov %bx, %ds
    mov %ss, %sp

    mov $len, %cx
1:
#    mov alphabetic(%ax), %bx #`alphabetic(%ax)' is not a valid base/index expression
    mov alphabetic(%bx), %ax
    inc %bx
    push %ax
    loop 1b
    mov $len, %cx
    mov $0, %bx
1:
    pop %ax
    mov %ax, alphabetic(%bx)
    inc %bx
    loop 1b
    jmp .
alphabetic:
    .ascii "abcdefghijklmnopqrstuvwxyz"
end:
    .set len, end - alphabetic #should not use $end - $alphabetic
#    .set len, alphabetic - $end #Error: invalid operands (.text and *UND* sections) for `-' when setting `len'