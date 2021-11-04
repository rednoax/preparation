ldr r0, =main
ldr pc, label
label:
	.word main

main:
	b main
.global _start
_start: //if no such: arm-none-eabi-ld: warning: cannot find entry symbol _start; defaulting to 0000000000008000
/*
$ qemu-arm label.out
qemu: uncaught target signal 4 (Illegal instruction) - core dumped
Illegal instruction (core dumped)
REASON: both `readelf -h` and gdb shows the entry point is at symbol _start. There is no valid instruction there.
*/
