/* 
.extern is accepted in the source program-for compatible with other assemblers-but
it is ignored. as treats all undefined symbols as external!
*/
    .text
.global start
start:
    ldr sp, =stack_top
    ldr r4, =a
    ldr r0, [r4]
    ldr r4, =b
    ldr r1, [r4]
    bl sum
    ldr r4, =c
    str r0, [r4]
stop: b stop
/*
    .data
a: .word 0xa5
b: .word 0x5a00
c: .word 0
*/