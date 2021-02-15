.text
.global reset
reset:
    ldr sp, =svc_stack
    mrs r0, cpsr
    bic r0, r0, #0x1f
    orr r0, r0, #0x12
    msr cpsr, r0
    ldr sp, =irq_stack
    bic r0, r0, #0xdf //clear IFT
    orr r0, r0, #0x13
    msr cpsr, r0
    bl copy_vector
    bl main
    b .

vector_start:
    ldr pc, reset_handler_addr
    ldr pc, undef_handler_addr
    ldr pc, swi_handler_addr
    ldr pc, prefetch_handler_addr
    ldr pc, data_handler_addr
    b .
    ldr pc, irq_handler_addr
    ldr pc, fiq_handler_addr
reset_handler_addr: .word reset
undef_handler_addr: .word undef_handler
swi_handler_addr:   .word swi_handler
prefetch_handler_addr:.word prefetch_handler
data_handler_addr:    .word data_handler
irq_handler_addr:    .word irq_handler
fiq_handler_addr:    .word fiq_handler
vector_end:

irq_handler:
    sub lr, lr, #4
    stmfd sp!, {r0-r10, fp, ip, lr}
    bl IRQ_handler
    ldmfd sp!, {r0-r10, fp, ip, pc}^

undef_handler:
swi_handler:
prefetch_handler:
data_handler:
fiq_handler:
    b .
