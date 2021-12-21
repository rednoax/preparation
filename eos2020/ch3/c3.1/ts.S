.text
#if 10
.code 32 //generate ARM instruction, rather than thumb
.global reset, vectors_start, vectors_end
reset:
    ldr sp, =svc_stack
    bl copy_vectors
    mrs r0, cpsr
    bic r1, r0, #0x1f
    orr r1, r1, #0x12// modes:irq
    msr cpsr, r1
    ldr sp, =irq_stack
    bic r0, r0, #0x80 //clear I as IF is all 1 by default
    msr cpsr, r0
    bl main
    b .

vectors_start:
    ldr pc, reset_handler_addr //use pc offset(+- 4KB) to keep a full 32bit address, so the @reset can be anywhere
    ldr pc, undef_handler_addr
    ldr pc, swi_handler_addr
    ldr pc, prefetch_abort_handler_addr
    ldr pc, data_abort_handler_addr
    b .
    ldr pc, irq_handler_addr
    ldr pc, fiq_handler_addr
reset_handler_addr: .word reset
undef_handler_addr: .word undef_handler
swi_handler_addr:   .word swi_handler
prefetch_abort_handler_addr:.word prefetch_abort_handler
data_abort_handler_addr:    .word data_abort_handler
irq_handler_addr:    .word irq_handler
fiq_handler_addr:    .word fiq_handler
vectors_end:

.align 2//the alignment ought to be very important
irq_handler:
    sub lr, lr, #4
    stmfd sp!, {r0-r10, fp, ip, lr}
    bl IRQ_handler
    ldmfd sp!, {r0-r10, fp, ip, pc}^

.align 2
undef_handler:
swi_handler:
prefetch_abort_handler:
data_abort_handler:
fiq_handler:
    b .

.global i, j, k
i:
j:
k: .word 0xdeadbeef
#endif
