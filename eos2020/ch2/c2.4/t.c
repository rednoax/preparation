extern int t0[];
extern int *t1;
extern int d0[];
extern int *d1;
/*
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
3. varible defined in c, accessed from .S, see 2.4.1's ts.s and t.c example. In brief it is the same as 2
.S: ldr r0,=a; ldr r0, [r0]
.c: int a = 0x80
4. varible defined in c, accessed from c, see get() example:
 one .data var or .bss var has no c language type stored in symbol table but only base address and size;
 c reading of var in .bss/.data consists of 2 step2: first get the symbol address(in .bss or .data) then read value from the address(extract a block value in .bss or .data)
*/
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

/*
readelf -s t.elf
    23: 000100bc     4 OBJECT  LOCAL  DEFAULT    2 var.1258<--a .data var or .bss var has no type in symbol table but base address and size
objdump -S t.elf
0001008c <get>:

int get()
{
   1008c:       e52db004        push    {fp}            ; (str fp, [sp, #-4]!)
   10090:       e28db000        add     fp, sp, #0      ; 0x0
        static int var = 1;
        return var;
   10094:       e59f3010        ldr     r3, [pc, #16]   ; 100ac <get+0x20>	<--1/2. get &var, which either in .bss or .data
   10098:       e5933000        ldr     r3, [r3]							<--2/2. *(&var)
}
   1009c:       e1a00003        mov     r0, r3
   100a0:       e28bd000        add     sp, fp, #0      ; 0x0
   100a4:       e8bd0800        pop     {fp}
   100a8:       e12fff1e        bx      lr
   100ac:       000100bc        .word   0x000100bc<--holds addresss of @var
*/
int get()
{
	static int var = 1;
	return var;
}


