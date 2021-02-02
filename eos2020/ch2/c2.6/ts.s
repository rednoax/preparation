    .text
    .code 32 //.arm is equvailent
    .global reset_start
reset_start:
    ldr sp, =stack_top
    bl main
    b .
