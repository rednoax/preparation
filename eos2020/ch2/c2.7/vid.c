#define WIDTH 640
#define ROW 480
#define FWIDTH 8
#define FROW 16
#define START_ROW 4
#define MAX_ROW (ROW / FROW)//30
#define MAX_COL (WIDTH / FWIDTH)//80
enum {
    BLUE, GREEN, RED,
};
extern char _binary_font_start[];
int color;
char cursor;
volatile int *fb;

int row, col;
char *font;
void fbuf_init()
{
    int i;
    fb = (int*)0x200000;
    font = _binary_font_start;
    *(volatile unsigned int *)(0x1000001c) = 0x2C77;
    *(volatile unsigned int *)(0x10120000) = 0x3F1F3F9C;
    *(volatile unsigned int *)(0x10120004) = 0x090B61DF;
    *(volatile unsigned int *)(0x10120008) = 0x067F1800;
    *(volatile unsigned int *)(0x10120010) = (int)fb;
    *(volatile unsigned int *)(0x10120018) = 0x82B;
    cursor = 127;
    for (i = 0; i < WIDTH * ROW; i++)
        fb[i] = 0x00ffffff;
}

void clrpix(int x, int y)//x/y all in pixel
{
    fb[x + WIDTH * y] = 0x0;
}

void setpix(int x, int y)
{
    int pos = x + WIDTH * y;
    int v = 0xffffff;
    swithc (color) {
    case RED: v = 0xff; break;
    case BLUE: v = 0xff << 16; break;
    case GREEN: v = 0xff << 8; break;
    }
    fb[pos] = v;
}
typedef void(*funcp)(int, int);
#define undchar(c, x, y) __w(c, x, y, clrpix)
#define dchar(c, x, y) __w(c, x, y, setpix)

void __w(int c, int x, int y, funcp f)//x/y is in pixel
{
    int i, j;
    //char *p =  font + c, l;
    char *p = font + c * 16, l;
    for (j = 0; j < FROW; j++) {//here the double loop is similar to progressive scan's double for loop
        l = p[j];
        for (i = 0; i < FWIDTH; i++) {
            if (l & (1 << i))//if (l & (1 << (7 - i)))
                f(x + i, y + j);
        }
    }
}

void scroll()
{
    int oy = START_ROW;
    int i, j;
    for (j = oy; j < ROW - oy - 2; j++)
        for (i = 0; i < WIDTH; i++)
            fb[i + WIDTH * j] = fb[i + WIDTH * (j + 1)];
    for (i = 0; i < WIDTH; i++)
        fb[i + WIDTH * j] = 0;
}
/*
lcd pixel:640x480 font pixel:8x16
font row range:[0, 480/16) => [0, 30)
font col range:[0, 640/8) => [0, 80)
I have known logo pic's pixel and font pixel, so it is able to display the font along with logo picture
and make nither of them overlay the other. So what's the purpose of using @row and @col below.
They are to implement the line feed, newline and screen scroll.
*/
void unkpchar(int c, int row, int col)
{
    int x, y;//both in pixel
    //to convert @row @col to pixel
    x = col * FWIDTH;
    y = row * FROW;
    undchar(c, x, y);
}

void kpchar(int c, int row, int col)
{
    int x, y;
    x = col * FWIDTH;
    y = row * FROW;
    dchar(c, x, y);
}

void clrcursor()
{
    unkpchar(127, row, col);
}

void putcursor()
{
    kpchar(127, row, col);
}

void kputc(int c)
{
    clrcursor();
    if (c == '\r') {
        col = 0;
        putcursor();
        return;
    }
    if (c == '\n') {
        row++;
        if (row >= MAX_ROW) {
            scroll();
            row = MAX_ROW - 1;
        }
        putcursor();
        return;
    }
    if (c == '\b') {//backspace is 0x8 but ugets return decimal 127, reason unknown.
        if (col > 0) {
            col--;
            putcursor();
        }
        return;
    }
    kpchar(c, row, col);
    col++;//
    if (col >= MAX_COL) {
        col = 0;
        row++;
        if (row >= MAX_ROW) {
            scroll();
            row = MAX_ROW - 1;
        }
    }
    putcursor();
}

void kprints(const char *s)
{
    while (*s) {
        kputc(*s++);
    }
}

void krpx(int i)
{
    if (i) {
        krpx(i / 16);
        kputc(tab[i % 16]);
    }
}

void kprintx(int i)
{
    kprints("0x");
    if (i == 0)
        kputc('0');
    else
        krpx(i);
    kputc(' ');
}

void krpu(int i)
{
    if (i) {
        krpu(i / 10);
        kputc(tab[i % 10]);
    }
}

void kprintu(int i)
{
    if (i == 0)
        kputc('0');
    else
        krpu(i);
    kputc(' ');
}

void kprinti(int i)
{
    if (i < 0) {
        kputc('-');
        i = -i;
    }
    kprintu(i);
}

void kprintf(const char *fmt, ...)
{
    const char *cp = fmt;
    int *ip = (int*)&fmt + 1;
    char c;
    while (c = *cp++) {
        if (c != '%') {
            kputc(c);
            if (c == '\n')
                kputc('\r');
            continue;
        }
        switch(*cp++) {
        case 'c':
            kputc(*ip++);
            break;
        case 's':
            kprints((const char*)*ip++);
            break;
        case 'd':
            kprinti(*ip++);
            break;
        case 'u':
            kprintu(*ip++);
            break;
        case 'x':
            kprintx(*ip++);
            break;
        }
    }
}