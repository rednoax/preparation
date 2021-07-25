#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int UART;
const char tab[] = "0123456789abcdef";
int uninit_var;//IT IS WEAK SYMBOL even its st_info is not 'WEAK', but 'GLOBAL'
int uninit_var2 __attribute__((weak));
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
a full disassembly is:
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
							@ high address --> low address
                                 sp
	stmfd	sp!, {fp, lr}	@|lr|fp|
	add	fp, sp, #4 @		  fp
	sub	sp, sp, #24			@|lr|fp|    |    |    |    |    |    | @shift sp downwards to allocate local variables and temp spaces on stack
	str	r0, [fp, #-8]		@|lr|fp|argc|                     sp				
	str	r1, [fp, #-12]      @|lr|fp|argc|argv|
	mov	r0, #0
	ldr	r1, .L36
	bl	uprintx
	ldr	r3, .L36+4
	str	r3, [sp]
	ldr	r3, .L36+8			@                          |.LC2|4660|
	str	r3, [sp, #4]<--string argument "fin" transfered to uprintf is a 4B address("fin"'s address, ie symbol value in .rodata) put on stack
	mov	r3, #16				@|lr|fp|argc|argv|?   | 16 |.LC2|4660|
	str	r3, [sp, #8]
	ldr	r0, .L36+12<--format string transfered to uprintf is a 4B address put in r0, ie ldr r0, =.LC1
	mov	r1, #49<--'1'(0x31) occupy 4B, not 1B even it is a char!then uprintf push the 4B,not 1B on stack!
	mov	r2, #23
	mov	r3, #45 			@now r[0,3] holds .LC1,'1',23,45,
	bl	uprintf
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4			@adjust sp
	@ sp needed
	ldmfd	sp!, {fp, lr}	@pop stack and return to caller
	bx	lr
.L37:
	.align	2
.L36:
	.word	43981
	.word	4660
	.word	.LC2
	.word	.LC1

*/
    uprintf("[%c %d %u %x %s %x]", '1', 23, 45, 4660/*0x1234*/, "fin", 16);//16 is at the stack higher and format string will be at the lower stack
    return 0;
}
/*
	.align	2
	.global	_printf
	.type	_printf, %function
_printf:
	@ Function supports interworking.
	@ args = 4, pretend = 16, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
	@ link register save eliminated.
	stmfd	sp!, {r0, r1, r2, r3}<--c function's args from left to right corresponds to stack lower to higher
	str	fp, [sp, #-4]!	@|r3|r2|r1|r0|fp|
	add	fp, sp, #0		@             fp
	sub	sp, fp, #0		@             sp
	@ sp needed
	ldr	fp, [sp], #4
	add	sp, sp, #16
	bx	lr
	.size	_printf, .-_printf
	.ident	"GCC: (Sourcery CodeBench Lite 2013.11-24) 4.8.1"
*/
void _printf(const char *fmt, ...)
{
}
