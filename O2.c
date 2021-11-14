/*
an example to show O2 that is used in __tlb_flag of kernel
gcc -Wall O2.c
/tmp/ccvD8rcl.o: In function `main':
main.c:(.text+0x34): undefined reference to `_arg'
collect2: error: ld returned 1 exit status
gcc -O2 -Wall O2.c or -O1 will not report error!
*/
#include <stdlib.h>
extern unsigned int _arg;
static volatile int g_i = 4;
int main(int argc, char** argv)
{
	int arg = 0;
asm volatile("1:");
	g_i = 1;//it adds a new entry in .rel.text, which record g_i symbol value ie '&g_i' in c lang,see comment A
asm volatile("2:");
	if (argc > 1)
		arg = atoi(argv[1]);
	if ( arg || (_arg & arg))
		return -1; 
	return 0;
}
/*comment A
$ . ~/opt/src/mini240/tc.sh
$ arm-none-linux-gnueabi-gcc -O2 -Wall O2.c
$ arm-none-linux-gnueabi-objdump -d a.out
...
00008370 <main>:
    8370:       e92d4010        push    {r4, lr}
    8374:       e59f3030        ldr     r3, [pc, #48]   ; 83ac <main+0x3c><==r3 <= &g_i
    8378:       e3a02001        mov     r2, #1  ; 0x1
    837c:       e5832000        str     r2, [r3]
    8380:       e1500002        cmp     r0, r2
    8384:       da000006        ble     83a4 <main+0x34>
    8388:       e5910004        ldr     r0, [r1, #4]
    838c:       e2822009        add     r2, r2, #9      ; 0x9
    8390:       e3a01000        mov     r1, #0  ; 0x0
    8394:       ebffffc8        bl      82bc <_init+0x44>
    8398:       e3500000        cmp     r0, #0  ; 0x0
    839c:       13e00000        mvnne   r0, #0  ; 0x0
    83a0:       18bd8010        popne   {r4, pc}
    83a4:       e3a00000        mov     r0, #0  ; 0x0
    83a8:       e8bd8010        pop     {r4, pc}
    83ac:       00010554        .word   0x00010554<==readelf -a:    63: 00010554     4 OBJECT  LOCAL  DEFAULT   22 g_i
...
Disassembly of section .data:
....
00010554 <g_i>:
   10554:       00000004        .word   0x00000004<==
Disassembly of section .bss:
...
*/