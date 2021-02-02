#include "uart.c"
#include "vid.c"
extern char _binary_panda_bmp_start[];
int main()
{
    const char *p;
    //UART *up = &uart[0]; 
    int mode = 0;
    uart_init();
    fbuf_init(mode);
    p = _binary_panda_bmp_start;
    show_bmp(p, 0, 80);
    while (1)
        ;
    return 0;
}