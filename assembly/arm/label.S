.globl _start
_start://if no symbol _start, it's also ok but there is warning: cannot find entry symbol _start; defaulting to 0000000000008000
	mov r0, #4
	mov r1, $4
	cmp r0, r1
	ldr r0, =main
	ldr r1, label @equals to the above
main:
	ldr r1, =dstart
	ldr r2, =dend
call:
	ldr r0, [r1], #4
	cmp r1, r2
	ldr lr, =call
	blo test @cc:us <
	bhs .
test:
	cmp r0, #1
	mov pc, lr
label:
	.word main
dstart:
	.word 0, 1, 2 @Z:0, 1, 1
dend: