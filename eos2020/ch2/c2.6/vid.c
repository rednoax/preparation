#define FB_BASE (6 * 1024 * 1024)
volatile int *fb = (int*)FB_BASE;
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

void show_bmp(const char *p, int startRow, int startCol)
{
    int size = *(int*)(p + 14);
    int w = *(int*)(p + 18);
    int h = *(int*)(p + 22);
    uprintf("%d %dx%d\n", size, w, h);
}