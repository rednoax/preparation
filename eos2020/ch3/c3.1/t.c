const char tab[]="0123456789abcdef";

#include "defines.h"
#include "vid.c"
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
    extern int*vectors_start, *vectors_end;
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
