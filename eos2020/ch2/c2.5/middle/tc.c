#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int UART;
const char tab[] = "0123456789abcdef";
void uputc(UART *up, int c)
{
    printf("%c", c);
}

void uprints(UART *up, const char *s)
{
    while (*s) {
        uputc(up, *s++);
    }
}
#if 10
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
#else
int urpx(UART *up, int x)
{
  char c;
  if (x) {
    c = tab[x % 16];
    urpx(up, x / 16);
    uputc(up, c);
  }
  //uputc(up, c);//should not be this for c can be uninitialized for the lat loop after the most significant number
}

int uprintx(UART *up, int x)
{
  uprints(up, "0x");
  if (x==0)
    uputc(up, '0');
  else
    urpx(up, x);
  uputc(up, ' ');
}
#endif

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
to make the following work, add -m32 to gcc!
That's why va_xx macros are needed, as the way below is only apply to 32bit arch.
*/
void uprintf(char *fmt, ...)
{
    char c;
    UART *up = NULL;//&uart[0];
    const char *fp = fmt;
    int *sp = (int*)(&fmt + 1);//fmt is char**, so to make stack go up, +1 rather than +4
    while ((c = *fp++)) {
        if (c == '%') {
            switch (*fp++) {
                case 'c':
                    uputc(up, (char)*sp++);
                    break;
                case 'd':
                    uprinti(up, *sp++);
                    break;
                case 'u':
                    uprintu(up, *sp++);
                    break;
                case 'x':
                    uprintx(up, *sp++);
                    break;
                case 's'://*sp here is an address,eg 0x56557074,
                    uprints(up, (const char *)(*sp++));
                    break;
            }
        } else
            uputc(up, c);
    }
}
int main(int argc, char **argv)
{
    uprintx(NULL, 43981);
/*
the following 2 string is in .rodata
	.section	.rodata<--
	.align	2
.LC1:
	.ascii	"[%c %d %u %x %s %x]\000"<--
	.align	2
.LC2:
	.ascii	"fin\000"<--
	.text<--
	.align	2
	.global	main
    ...
then their addresses are put in reg or stack as argument, which are transfered to uprintf:
in real world, their address is referenced by a label .L36, plus offset
.L36:
	.word	43981
	.word	4660
	.word	.LC2
	.word	.LC1
*/
    uprintf("[%c %d %u %x %s %x]", '1', 23, 45, 4660/*0x1234*/, "fin", 16);
    return 0;
}
