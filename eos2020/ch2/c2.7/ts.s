    .text
.code 32
    .globl reset_start
reset_start:
    ldr sp, =stack_top
    bl main
    b .
