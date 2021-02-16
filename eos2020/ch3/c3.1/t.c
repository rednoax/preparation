const char tab[]="0123456789abcdef";

#include "vid.c"

void IRQ_handler()
{

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
    extern int i, j[], *k;//i and k will refer to symbol value, ie address, but j will not refer to symbol value
	//*k is wrong usage as it refers twice, 1st k is actually *(symbol value ie address in table) so k is 0xdeadbeef, then *k is *(0xdeadbeef) that gets 0
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
    color = RED;
    row = col = 0;
    fbuf_init();
    //symbol_test();
    return 0;
}
