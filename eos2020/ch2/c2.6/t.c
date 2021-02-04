#include "uart.c"
#include "vid.c"
extern char _binary_panda_bmp_start[], _binary_porkcar_bmp_start[];
int main()
{
    const char *p;
    UART *up = &uart[0]; 
    int mode = 0;
    uart_init();
    fbuf_init(mode);
    p = _binary_panda_bmp_start;
    show_bmp(p, 0, 80);
    while (1) {
        uprintf("enter a key from this UART : ");
        ugetc(up);
        p = _binary_porkcar_bmp_start;
        show_bmp(p, 120, 0);
        uprintf("enter a key from this UART : ");
        ugetc(up);
        p = _binary_panda_bmp_start;
        show_bmp(p, 120, 80);
        uprintf("enter a key from this UART : ");
        ugetc(up);
        p = _binary_porkcar_bmp_start;
        show_bmp(p, 0, 0);
        uprintf("enter a key from this UART : ");
        ugetc(up);
    }
    while (1) 
        ;
    return 0;
}