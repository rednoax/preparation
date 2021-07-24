extern int t0[];
extern int *t1;
extern int d0[];
extern int *d1;
#if 0
readelf -s
    23: 0001008c     0 NOTYPE  GLOBAL DEFAULT    2 d1
    24: 00010024   104 FUNC    GLOBAL DEFAULT    1 sum
    25: 0001008c     0 NOTYPE  GLOBAL DEFAULT    2 d0<--no difference with d1, their value is VMA, but d1 will be referenced in c if claimed as the above
    26: 00011098     0 NOTYPE  GLOBAL DEFAULT  ABS stack_top
    27: 00010018     0 NOTYPE  GLOBAL DEFAULT    1 t1<--similar to relation between d0 and d1
    28: 00010018     0 NOTYPE  GLOBAL DEFAULT    1 t0

(gdb) target remote:1234
Remote debugging using :1234
0x00000000 in ?? ()
(gdb) p t0<--only int[] can reserve symbol value
$21 = 0x10018
(gdb) p t1<--any other forms except int[] becomes symol's reference
$22 = (int *) 0xeafffffe
(gdb) p d0<--only int[] can reserve symbol value
$23 = 0x1008c
(gdb) p d1
$24 = (int *) 0xa5
(gdb) x/x 0x10018<--symbol t1 is 0x10018, show its reference
0x10018 <t1>:   0xeafffffe
(gdb) x/x 0x1008c<--symbol d1 is reference of its symbol
0x1008c <d1>:   0x000000a5
(gdb) p &d1
$25 = (int **) 0x1008c<--to get the symbol d1's value, use &d1
(gdb) p &t1
$26 = (int **) 0x10018<--ditto
(gdb)
to summarize:
1. in .c, if reading a variable defined in .S, no matter the variable in .text or .data, just claim the 
variable's label in any form except an array name,eg t1 or d1 above, then their value is referenced symbol. Use
&label if you want the label value.
2. in .c, if u want a symbol value in .S, use type[].
How to access varible
1. varible defined in .S, accessed from c:
.S:.global a
a: .word 0xa5
.c: extern int a(or any other claim like 'extern int *a;', but never 'extern int a[]'), then a==0xa5
2. varible defined in .S, accessed from .S, see 2.4's ts.s, just like
ldr r0,=a
ldr r0,[r0]
a: .word 0xa5
3. varible defined in c, accessed from .S, see 2.4.1's ts.s and t.c example.
4. varible defined in c, accessed from c: ask gcc to do it.
#endif
int sum(int x, int y)
{
	//
	volatile int *a;
	a = t0;
	a = t1;
	a = d0;
	a = d1;
	//
    return x + y;
}


