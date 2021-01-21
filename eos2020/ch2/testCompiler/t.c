/*
arm-none-eabi-gcc -S t.c -o t.s -Wall -mcpu=arm926ej-s
*/
extern int g(int x, int y);
int func(int a, int b, int c, int d, int e)
{
    int x, y, z;
    x = 1; y = 2; z = 3;
    g(x, y);
    return a + e;
}