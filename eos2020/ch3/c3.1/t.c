const char tab[]="0123456789abcdef";

#include "defines.h"
#include "string.c"
#include "vid.c"
#include "timer.c"

TIMER *tp[4];
void IRQ_handler()
{
	int vicstatus = VIC_STATUS;
	if (vicstatus & (1 << 4)) {//timer 0,1
		if (tp[0]->base->value == 0)//.value give current value of the decrementing counter
			timer_handler(0);
		if (tp[1]->base->value == 0)
			timer_handler(1);
	}
	if (vicstatus & (1 << 5)) {
		if (tp[2]->base->value == 0)
			timer_handler(2);
		if (tp[3]->base->value == 0)
			timer_handler(3);
	}
}

void copy_vectors()
{
    extern int vectors_start[], vectors_end[];
    const int *src = vectors_start;
    int *dst = (int*)0;
    while (src < vectors_end)
        *dst++ = *src++;    
}

void symbol_test()
{
/*
i refer once which is equivalent to *(i's symbol value ie one address), so to get the symbol value &i should
be used.
k is the same as i even its typeof is int*.
i and k will refer to symbol value, which is an address, but j will not refer to symbol value, j
is exactly == symbol value in symbol table.
*k is wrong usage as it refers twice, 1st k is actually *(symbol value ie address in table) so k is 0xdeadbeef, then *k is *(0xdeadbeef) that gets 0
*/
    extern int i, j[], *k;
	kprintf("%x==%x!=%x; %x==%x!=%x\n", &i, j, k, i, *j, *k);
    i = 0xfaceb00c;
    kprintf("%x==%x==%x\n", i, *j, *k);
    *j = 0xcafecafe;
    kprintf("%x==%x==%x\n", i, *j, *k);
	*k = 0x80868086;
    kprintf("%x==%x==%x\n", i, *j, *k);
}
#if 0
/*
1.if '-mabi=apcs-gnu' is used, the divide function name is changed from __aeabi_idiv to __divsi3.
If the following empty __divsi3 is added, the `gcc -c t.c` will be ok and their objdump is shown below:
00001288 <__divsi3>:
    1288:       e52db004        push    {fp}            ; (str fp, [sp, #-4]!)
    128c:       e28db000        add     fp, sp, #0
    1290:       e24dd004        sub     sp, sp, #4
    1294:       e50b0004        str     r0, [fp, #-4]
    1298:       e3a03000        mov     r3, #0
    129c:       e1a00003        mov     r0, r3
    12a0:       e28bd000        add     sp, fp, #0
    12a4:       e49db004        pop     {fp}            ; (ldr fp, [sp], #4)
    12a8:       e12fff1e        bx      lr

000012ac <divide>:
    12ac:       e92d4810        push    {r4, fp, lr}
    12b0:       e28db008        add     fp, sp, #8
    12b4:       e24dd004        sub     sp, sp, #4
    12b8:       e50b000c        str     r0, [fp, #-12]
    12bc:       e3a03000        mov     r3, #0
    12c0:       e5933000        ldr     r3, [r3]
    12c4:       e3a04a01        mov     r4, #4096       ; 0x1000
    12c8:       e1a01003        mov     r1, r3
    12cc:       e51b000c        ldr     r0, [fp, #-12]
    12d0:       ebfffffe        bl      1288 <__divsi3>
    12d4:       e1a03000        mov     r3, r0
    12d8:       e5843000        str     r3, [r4]
    12dc:       e1a00000        nop                     ; (mov r0, r0)
    12e0:       e24bd008        sub     sp, fp, #8
    12e4:       e8bd8810        pop     {r4, fp, pc}
2.if ts.S and t.c is compiled with different -mabi option, the link will fail:
++ arm-none-eabi-gcc -c -g -Wall -mcpu=arm926ej-s ts.S -o ts.o <=-mabi=aapcs-linux is used by default
++ arm-none-eabi-gcc -c -g -Wall -mcpu=arm926ej-s -mabi=apcs-gnu t.c -fno-builtin -o t.o <= -mabi=apcs-gnu is explicitly used
++ func arm-none-eabi-ld -T t.ld ts.o t.o -Map t.map -o t.elf
++ arm-none-eabi-ld -T t.ld ts.o t.o -Map t.map -o t.elf
arm-none-eabi-ld: error: source object t.o has EABI version 0, but target t.elf has EABI version 5<=different abi cause link error

*/
int __divsi3(int i)
{
	return 0;
}
/*
if no '-mabi=apcs-gnu' during `gcc -c`, '-mabi=aapcs-linux' is used by default.
The following is error when no '-mabi' option is used:
1. if no '-lgcc -L $(dirname `arm-none-eabi-gcc -print-libgcc-file-name`)' in ld, there is error:
t.c:60: undefined reference to `__aeabi_idiv'
2. change / to %:
/home/rednoah/opt/preparation/eos2020/ch3/c3.1/t.c:61: undefined reference to `__aeabi_idivmod'
3. if changing to unsigned divide:
undefined reference to `__aeabi_uidiv'
*/
void divide(int i)
{
	*(int*)0x1000 = i / *(int*)0;
}
#endif
int main()
{
	int i;
    color = RED;
    row = col = 0;
    fbuf_init();
    //symbol_test();
    /*
    IRQ4: timer 0 and 1
    IRQ5: timer 2 and 3
	*/
    VIC_INTENABLE |= (1 << 4) | (1 << 5);//IntEnable: write 0 has no effect, write 1 to enable irq
    kprintf("Program C3.1 start: test timer driver by interrupts\n");
	timer_init();
	for (i = 0; i < ARRAY_SIZE(timer); i++) {
		tp[i] = &timer[i];
		timer_start(i);
	}
	kprintf("Enter while(1) loop\n");
    return 0;
}
