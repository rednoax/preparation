	.text
.global start
start:
	ldr sp, =stack_top
	stmfd sp!, {r0-r3, lr}
	bl sum
stop:
	b stop
sum:
	mov r0, #0
	ldr r1, =Array @as long as ldr Rd,=label arises, there CERNTAINLY will be mem to/from reg ops since the label is just mem adress that is used to transfer data between mem and reg.ARM cannot directly change mem so if it want to change mem: it must load data from mem to reg, change the reg then write it back to mem
	ldr r2, =N
	ldr r2, [r2]
sum1:
	ldr r3, [r1], #4
	add r0, r0, r3
	subs r2, r2, #1
	bne sum1
	ldr r1, =Result
	str r0, [r1]
	ldmfd sp!, {r0-r3, pc}
	.data
N:		.word 10
Array:	.word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
Result:	.word 0
/*
$ arm-none-eabi-nm t.elf
00010050 d Array
0001004c d N
00010078 d Result
use above to get Result's address, then c-a c to enter monitor to check mem content of the address
(qemu) xp/8w 0x10078
0000000000010078: 0x00000037 0x00000000 0x00000000 0x00000000
0x37 is 55, so it is right
 */