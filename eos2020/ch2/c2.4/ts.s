/* 
.extern is accepted in the source program-for compatible with other assemblers-but
it is ignored. as treats all undefined symbols as external!
*/
    .text
.global start, t0, t1, d0, d1 @t0 t1 in .text and d0 d1 in .bss. They will be claimed in different type in c
start:
    ldr sp, =stack_top
    ldr r4, =a @ to access a mem spanning sections, whose address is @a in .data), ldr reg,=label seems the easiest way
    ldr r0, [r4], #4
    ldr r1, [r4], #4
    bl sum
    str r0, [r4]
t0:
t1:
stop: b stop
    .data
d0:
d1:
a: .word 0xa5
b: .word 0x5a00
c: .word 0
