.globl _start
_start:
	mrs r0,cpsr
	bic r0,r0,#0x1f @clear modes field
	orr r0,r0,#0xd3 @disable irq/fiq, arm state, SVC mode
	b main
