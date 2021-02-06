#define ARRAY_SIZE(x) ((sizeof(x))/(sizeof((x)[0])))
#define TXFF 0x20
#define RXFE 0x10
#define DR 0x00
#define FR 0x18
typedef struct uart {
    volatile char *base;
    int n;
} UART;

UART uart[4];
const char tab[] = "0123456789abcdef";

void uart_init()
{
    int i;
    for (i = 0; i < ARRAY_SIZE(uart); i++) {
        uart[i].base = (char*)(0x101f1000 + i * 0x1000);
        uart[i].n = i;
    }
    uart[3].base = (char*)0x10009000;
}

void uputc(UART *up, char c)
{
    if (c == '\n')//borrowed from u-boot
        uputc(up, '\r');
    while (up->base[FR] & TXFF) ;
    up->base[DR] = c;
}

int ugetc(UART *up)
{
    while (up->base[FR] & RXFE) ;
    return up->base[DR];
}

void ugets(UART *up, char *s)
{
    while ((*s = ugetc(up)) != '\r') {//enter is \r, only 1B
        uputc(up, *s);
        s++;
    }
    *s = '\0';
}
//uputs is not used so it is removed
void uprints(UART *up, const char *s)
{
    while (*s) {
        uputc(up, *s++);
    }
}

void urpx(UART *up, int x)
{
    if (x / 16)
        urpx(up, x / 16);
    uputc(up, tab[x % 16]);
}

void uprintx(UART *up, int x)
{
    uprints(up, "0x");
    if (x == 0)
        uputc(up, '0');
    else
        urpx(up, x);
    uputc(up, ' ');
}

void urpu(UART *up, int x)
{
    if (x / 10)
        urpu(up, x / 10);
    uputc(up, tab[x % 10]);
}

void uprintu(UART *up, int x)
{
    if (x == 0)
        uputc(up, '0');
    else
        urpu(up, x);
    uputc(up, ' ');
}

void uprinti(UART *up, int x)
{
    if (x < 0) {
        uputc(up, '-');
        x = -x;
    }
    uprintu(up, x);
}
/*
if make uprint nop, gcc -S show the empty uprintf's 1st line: stmfd	ip!, {r0, r1, r2, r3}
*/
void uprintf(char *fmt, ...)
{
    char c;
    UART *up = &uart[0];
    char *cp = fmt;
    int *ip = (int*)&fmt + 1;//NOT +4
    while ((c = *cp++)) {
        if (c == '%') {
            switch (*cp++) {
                case 'c':
/*++ is higer than *,but the actual increment/decrement of the operand is delayed,so
*p++ is *ip first then ip++
typecast and dereference * is at the same level, but associativity is from right-to-left, so
(char)(*ip) is not needed!
*/
                    uputc(up, (char)*ip++);
                    break;
                case 'd':
                    uprinti(up, *ip++);
                    break;
                case 'u':
                    uprintu(up, *ip++);
                    break;
                case 'x':
                    uprintx(up, *ip++);
                    break;
/*printf("%s", "abc");
NOTE "abc" is in .rodata and its symbol value ie a 4B address is put on the stack for uprintf
see c2.5/middle/t.c
*/
                case 's':
                    uprints(up, (const char *)*ip++);
                    break;
            }
        } else {
            uputc(up, c);
            /*
            if (c == '\n')
                uputc(up, '\r');
            */
        }
    }
}