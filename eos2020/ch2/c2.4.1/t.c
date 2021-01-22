/*
objdump -D t.o
a/b in .data, c in .bss
.data:
a: .word 0x80
b. .word 0x8080
hexdump -C t.bin
b is the last byte in t.bin, no c in t.bin!
*/
int a = 0x80, b = 0x8080, c = 0;
int sum(int x, int y)
{
    return x + y;
}