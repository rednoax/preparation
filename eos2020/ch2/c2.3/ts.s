.global reset, sum
    .text
reset:
    ldr sp, =stack_top
    b main
//int sum(int a, int b, int c, int d, int e, int f)
sum:
    stmfd sp!, {fp, lr}
    add fp, sp, #4

    add r0, r0, r1
    add r0, r0, r2
    add r0, r0, r3
    ldr r1, [fp, #4]
    add r0, r0, r1
    ldr r1, [fp, #8]
    add r0, r0, r1

    sub sp, fp, #4
    ldmfd sp!, {fp, pc}
    .section test //this section will be put after .bss if t.ld doesnot handle section test at all
.word 0
