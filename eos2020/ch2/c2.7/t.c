#include "uart.c"
#include "vid.c"

extern char _binary_logo_start[];

void show(const char *p, int startRow, int startCol)
{
    int h, w, x, y;
    h = (p[3] << 24) + (p[2] << 16) + (p[1] << 8) + p[0];
    p += 4;
    w = (p[3] << 24) + (p[2] << 16) + (p[1] << 8) + p[0];
    p += 4;
    //uprintf("h=%d w=%d\n", h, w);
    for (y = startRow; y < startRow + h; y++) {
        for (x = startCol; x < startCol + w; x++) {
            fb[x + WIDTH * y] = (p[2] << 16) + (p[1] << 8) + p[0];
            p += 3;
        }
    }
}
//#define kprintf uprintf//just for debug
int main()
{
    char line[64];
    uart_init();
    fbuf_init();
    show(_binary_logo_start, 0, 0);
    row = START_ROW;
    uprintf("%s %x %x %d %u %x, %c, %s\n", "test", 0xab, 0xdeadbeef, -1580, 1580, 10240, 'a', "fin");
    while (1) {
        color = GREEN;
        kprintf("enter a line from UART port : ");
        uprintf("enter line from UART : ");
        ugets(line);
        uprintf(" line=%s\n", line);
        color = RED;
        kprintf("line=%s\n", line);
    }
    return 0;
}