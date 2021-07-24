/* 
.extern is accepted in the source program-for compatible with other assemblers-but
it is ignored. as treats all undefined symbols as external!
*/
    .text
.global start
start:
    ldr sp, =stack_top
    ldr r4, =a @ to access a mem spanning sections, whose address is @a in .data), ldr reg,=label seems the easiest way
    ldr r0, [r4], #4
    ldr r1, [r4], #4
    bl sum
    str r0, [r4]
stop: b stop
    .data
a: .word 0xa5
b: .word 0x5a00
c: .word 0
