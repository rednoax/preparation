/*ts.s file of C2.1*/

.text
	.global start
start:
	mov r0, #1
	mov r1, #2
	add r1, r1, r0
	ldr r2, =result @both ts.o and ts.elf: ldr r2, [pc, #4], see comment 1
	str r1, [r2]
stop:
	@adr r3, result @r3 = pc + offset, BUT: ts.s:12: Error: symbol .data is in a different section
	@ldr r3, [r3]
	@ldr r4, result @r4 = [pc + offset], BUT: ts.s:14: Error: internal_relocation (type: OFFSET_IMM) not fixed up
	b stop
@.data:@'arm-non-eabi-as -o ts.o -c ts.s' emits: ts.s:13: Error: symbol `.data' is already defined
.data @if removed, arm-none-eabi-objdump -t ts.o can still display .text/.data/.bss in SYMBOL TABLE, they are predefined section name by as
result:
	.word 0
//
	.section full
dummy:
	.word 0
//
/*comment 1
arm has cannot directly modify the conent in mem, while x86 can do it in one single instrcution.
When arm needs to modify content of memory, it may need to get the address of the mem in ways like:
1. ldr reg, =label
2. adr reg, label<-cannot span section, see error of the above
Also,  arm can bypass getting mem address directly by loading reg with mem content like:
1. ldr reg, label<-cannot span section, see error of the above

label above can be defined in .[sS] (eg result above)or ld script file()

*/