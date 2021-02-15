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

int main()
{
    color = RED;
    row = col = 0;
    fbuf_init();
    return 0;
}