/*ts.s file of C2.1*/

.text
	.global start
start:
	mov r0, #1
	mov r1, #2
	add r1, r1, r0
	ldr r2, =result @both ts.o and ts.elf: ldr r2, [pc, #4], [pc,#4] holds LABEL result's address,see comment 1
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
$ arm-none-eabi-objdump -d t.elf

t.elf:     file format elf32-littlearm

Disassembly of section .text:

00010000 <start>:
   10000:       e3a00001        mov     r0, #1
   10004:       e3a01002        mov     r1, #2
   10008:       e0811000        add     r1, r1, r0
   1000c:       e59f2004        ldr     r2, [pc, #4]    ; 10018 <stop+0x4>
   10010:       e5821000        str     r1, [r2]

00010014 <stop>:
   10014:       eafffffe        b       10014 <stop>
   10018:       0001001c        .word   0x0001001c<--store synbol 'result' value: ie its address

$ arm-none-eabi-nm t.elf
00010020 n dummy
0001001c d result
00010000 T start
00010014 t stop

arm has cannot directly modify the conent in mem, while x86 can do it in one single instrcution.
When arm needs to modify content of memory, it may need to get the address of the mem in ways like:
1. ldr reg, =label
2. adr reg, label<-cannot span section, see error of the above
Also,  arm can bypass getting mem address directly by loading reg with mem content like:
1. ldr reg, label<-cannot span section, see error of the above

label above can be defined in .[sS] (eg result above)or ld script file()

*/