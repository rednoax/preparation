#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

static int i = 0;
static int fd = -1;

void write_file(char *buf, int size)
{
    if (fd == -1) {
        char filename[32];
        sprintf(filename, "%d.log", i++);
        fd = open(filename, O_CREAT | O_RDWR, 0644);
        printf("create %s: %d\n", filename, fd);
    }
    write(fd, buf, size);
}

void close_file()
{
    if (fd >= 0) {
        close(fd);
        fd = -1;
    }
}

int show_bmp(char *p, int startRow, int startCol)
{ 
    char buf[1024];
  int h, w, pixel, r1, r2, i, j; 
   unsigned char r, g, b;
   char *pp;
 
   int *q = (int *)(p+14); // skip over 14 bytes file header 
   q++;                    // skip 4 bytes in image header
   w = *q;                 // width in pixels 
   h = *(q + 1);           // height in pixels

   p += 54;                // p point at pixels now 

   // but the picture is up-side DOWN

   r1 = 3*w;
   r2 = 4*((3*w+3)/4);     // row size is a multiple of 4 bytes  
    printf("%dx%d rsize %d\n", w, h, r2);

   //p = (char *)(pp + (h-1)*r2);     // 3 bytes per pixel
   // p += (h-1)*r2;
   // p += h*r2;
   p += (h-1)*r2;
   for (i=startRow; i<h+startRow; i++){
     pp = p;
     // for (j=startCol; j<w+startCol; j++){
     for (j=startCol; j<startCol+w; j++){
         b = *pp; g = *(pp+1); r = *(pp+2);
         pixel = (b<<16) + (g<<8) + r;
         //fb[i*640 + j] = pixel;
         sprintf(buf, "[%d] %x@%p\n", i * 640 + j, pixel, pp);
         write_file(buf, strlen(buf));
         pp += 3;    // back pp by 3 bytes
     }
    sprintf(buf, "====%d %d\n", i, j);
    write_file(buf, strlen(buf));
     p -= r2;
   }
   /*********
   row = 20;
   kprintf("h=%d w=%d r1=%d r2=%d\n", h, w, r1, r2);
   ******/
   //uprintf("\nBMP image height=%d width=%d\n", h, w);
   close_file();
}

void show_bmp2(const char *pD, int startRow, int startCol)
{
    int size = *(int*)(pD + 14);
    int wp = *(int*)(pD + 18);//width in pixel
    int hp = *(int*)(pD + 22);//height in pixel
    const char *image = pD + 54, *p, *pp;
    int x, y, rsize, data;//, pos = startCol + 640 * startRow;
    char buf[1024];
    rsize = ((3 * wp + 3) / 4) << 2;
    printf("%d %dx%d rsize %d\n", size, wp, hp, rsize);
    p = image + (hp - 1) * rsize;
    for (y = startRow; y < startRow + hp; y++) {
        pp = p;
        for (x = startCol; x < startCol + wp; x++) {
            data = (pp[2] & 0xff) + ((pp[1] & 0xff) << 8) + ((pp[0] & 0xff) << 16);
            //fb[pos++] = data;
            sprintf(buf, "[%d] %x@%p\n", x + 640 * y, data, pp);
            write_file(buf, strlen(buf));
            pp += 3;
        }
        sprintf(buf, "====%d %d\n", y, x);
        write_file(buf, strlen(buf));
        p -= rsize;
    }
    close_file();
}

int main()
{
    char buf[0x100000];
    int fd = open("../panda.bmp", O_RDONLY);
    read(fd, buf, sizeof(buf));
    close(fd);
    show_bmp(buf, 0, 80);
    show_bmp2(buf, 0, 80);
    return 0;
}