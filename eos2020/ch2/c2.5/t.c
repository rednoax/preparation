#include "string.c"
#include "uart.c"
int g_uninit;//readelf -s t.o show it is COMM, not consume room of .bss; readelf -a t.elf show g_uninit is in .bss
int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int main()
{
    char string[32];
    int i, sum;
    uart_init();
    UART *up = &uart[0];
    uprintf("Enter lines from UART terminal, enter quit to exit\n");
    while (1) {
        ugets(up, string);
/*readelf -x .rodata shows this string is in .rodata, after the above string:Enter lines...
there is no .rodata in t.ld, it is put after .text
*/
        uprintf("    ");
        if (strcmp(string, "quit") == 0)
            break;
        uprintf("%s\n", string);
    }
    uprintf("\n");
    uprintf("Computer sum of array\n");
    sum = 0;
    for (i = 0; i < ARRAY_SIZE(v); i++)
        sum += v[i];
    uprintf("sum = %d\n"
            "END OF UART DEMO\n"
            "Enter control-a, then x to exit QEMU\n", sum);

    return 0;
}