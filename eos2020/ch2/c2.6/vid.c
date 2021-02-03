#define FB_BASE (6 * 1024 * 1024)
volatile int *fb = (int*)FB_BASE;
extern char _binary_panda_bmp_end[];
void fbuf_init(int mode)
{
    int i;
    /********* for VGA 640x480 ************************/
    *(volatile unsigned int *)(0x1000001c) = 0x2C77;        // LCDCLK SYS_OSCCLK
    *(volatile unsigned int *)(0x10120000) = 0x3F1F3F9C;    // time0
    *(volatile unsigned int *)(0x10120004) = 0x090B61DF;    // time1
    *(volatile unsigned int *)(0x10120008) = 0x067F1800;    // time2
    *(volatile unsigned int *)(0x10120010) = FB_BASE; // panelBaseAddress
    *(volatile unsigned int *)(0x10120018) = 0x82B;         // control register
    /*
    0x00000000 white
    0x00ffffff black
    0x00ff0000 blue
    0x0000ff00 green
    0x000000ff red
    */
    for (i = 0; i < 640 * 480; i++)
        fb[i] = 0x0;
}

void show_bmp(const char *pH, int startRow, int startCol)
{
    //int size = *(int*)(pH + 14);
    int wp = *(int*)(pH + 18);//width in pixel
    int hp = *(int*)(pH + 22);//height in pixel
    const char *image = pH + 54, *p, *pp;
    int x, y, rb;
    //uprintf("%d %dx%d\n", size, wp, hp);
    rb = ((3 * wp + 3) / 4) << 2;
    p = image;
    for (y = startRow; y < startRow + hp; y++) {
        pp = p;
        for (x = startCol; x < startCol + wp; x++) {
            fb[x + y * 640] = (pp[2] & 0xff) + ((pp[1] & 0xff) << 8) + ((pp[0] & 0xff) << 16);
            pp += 3;
        }
        p += rb;
    }
}
