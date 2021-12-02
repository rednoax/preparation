.globl _start
_start://if no symbol _start, it's also ok but there is warning: cannot find entry symbol _start; defaulting to 0000000000008000
	mov r0, #4
	mov r1, $4
@cpsr: 0x10(user)
	cmp r0, r1
@cpsr: 0x60000010: ZC set now. Z:set if result==0. C:set if unsigned >=, cleared if unsigned <
	ldr r0, =main
	ldr r1, label @equals to the above
main:
	ldr r1, =dstart
	ldr r2, =dend
call:
	ldr r0, [r1], #4
	cmp r1, r2
	ldr lr, =call
	bls test	@ls: us <=
	bhi .		@hs: us >
test:
	cmp r0, #1
	mov pc, lr
label:
	.word main
dstart:
	.word 0, 1, 2 @Z:0, 1, 1
dend: