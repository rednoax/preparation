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
    extern int i, *j, k[];
    kprintf("%x==%x==%x %x==%x==%x\n", &i, j, k, i, *j, *k);
    i = 0xa5;
    kprintf("%x==%x\n", i, *j);
    *j = 0x5a;
    kprintf("%x==%x\n", i, *j);
}

int main()
{
    color = RED;
    row = col = 0;
    fbuf_init();
    symbol_test();
    return 0;
}