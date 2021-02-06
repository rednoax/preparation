#define DR 0x00
#define FR 0x18
typedef struct uart {
    volatile char *base;
    int n;
} UART;

char tab[] = "0123456789abcdef";
UART uart[4];
UART *up = uart + 0;
void uart_init()
{
    int i;
    for (i = 0; i < 4; i++) {
        uart[i].base = (char*)(0x101f1000 + i * 0x1000);
        uart[i].n = i;
    }
    uart[3].base = (char*)0x10009000;
}

void uputc(char c)
{
    if (c == '\n')
        uputc('\r');
    while (*(up->base + FR) & 0x20) ;
    *(up->base + DR) = c;
}

int ugetc()
{
    while (*(up->base + FR) & 0x10) ;
    return *(up->base + DR);
}
/*
when inputing '1' backspace '2' in different terminals, the return is different!
Desktop terminal return: '1', 127 decimal, '2', and backspace echo back in ugets is NOT displayed at all.
ssh moba terminal return:'1' '\b' '2', and backspace echo back in ugets is displayed but the old char will remain(not erased)
*/
void ugets(char *s)
{
    char c;
    while ((c = ugetc()) != '\r') {
        *s++ = c;
        uputc(c);
    }
    *s = 0;
}

void uprints(const char *s)
{
    while (*s) {
        uputc(*s++);
    }
}

void urpx(int i)
{
    if (i) {
        urpx(i / 16);
        uputc(tab[i % 16]);
    }
}

void uprintx(int i)
{
    uprints("0x");
    if (i == 0)
        uputc('0');
    else
        urpx(i);
    uputc(' ');
}

void urpu(int i)
{
    if (i) {
        urpu(i / 10);
        uputc(tab[i % 10]);
    }
}

void uprintu(int i)
{
    if (i == 0)
        uputc('0');
    else
        urpu(i);
    uputc(' ');
}

void uprinti(int i)
{
    if (i < 0) {
        uputc('-');
        i = -i;
    }
    uprintu(i);
}
void uprintf(const char *fmt, ...)
{
    char c;
    const char *cp = fmt;
    int *ip = (int*)(&fmt + 1);
    while ((c = *cp++)) {
        if (c == '%') {
            switch(*cp++) {
            case 'c':
                uputc(*(char*)ip++);//a char also occupies 4B, see c2.5/middle/tc.c
                break;
            case 's':
                uprints((const char*)*ip++);//a string arg is 4B address to refer the byte stream of string, see c2.5/middle/tc.c
                break;
            case 'd':
                uprinti(*ip++);
                break;
            case 'u':
                uprintu(*ip++);
                break;
            case 'x':
                uprintx(*ip++);
                break;
            }
        } else {
            uputc(c);
        }
    }
}