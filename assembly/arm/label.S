_start://if no symbol _start, it's also ok but there is warning: cannot find entry symbol _start; defaulting to 0000000000008000
	ldr r0, =main
	ldr r1, label @equals to the above
main:
	b .
label:
	.word main