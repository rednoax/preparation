/*
arm-none-linux-gnueabi-gcc -Wall -c tc.c
arm-none-linux-gnueabi-gcc -Wall -c d.c

arm-none-linux-gnueabi-readelf -a d.o|less
//uninitialized global(non static) var is weak, belongs to COM in .o, not in .bss of .o.
     8: 00000008     8 OBJECT  GLOBAL DEFAULT  COM uninit_var<--COM is weak even its st_info is 'GLOBAL' not 'WEAK', st_value in .o:1.non COM symbol:offset in section;2.COM symbol:alignment;
     9: 00000008     8 OBJECT  GLOBAL DEFAULT  COM uninit_var2<--ditto,so 8B aligned, size is 8(long long)

arm-none-linux-gnueabi-readelf -a tc.o|less
//to make st_info "WEAK", claim it like uninit_var2 with __attribute__((weak))
    26: 00000004     4 OBJECT  GLOBAL DEFAULT  COM uninit_var<--4B aligned, size is 4B, weak symbol
    27: 00000000     4 OBJECT  WEAK   DEFAULT    4 uninit_var2<--st_value for a non COMM symbol is offset in section 4(ie .bss),weak symbol

so all uninitialized global var in d.c and tc.c are all "in COM block"!
arm-none-linux-gnueabi-gcc -Wall tc.c d.c -o a.out  <--no err as uninit global var is WEAK!

arm-none-linux-gnueabi-readelf -a a.out|less
// in executable elf:st_value is VMA, and all uninit vars are in .bss, no COM any more!
    87: 00010a40     8 OBJECT  GLOBAL DEFAULT   23 uninit_var<---8B in .bss([23] is .bss)
    88: 00008468    96 FUNC    GLOBAL DEFAULT   12 uprintx
    89: 000088d0     0 FUNC    GLOBAL DEFAULT   13 _fini
    90: 000082bc   512 FUNC    GLOBAL DEFAULT  UND putchar@@GLIBC_2.4
    91: 00010a48     8 OBJECT  GLOBAL DEFAULT   23 uninit_var2<--ditto

IN BRIEF:
1.uninit_var is uninitialized global var, defined both in tc.c and d.c. Both of them is weak! IF ALL ARE WEAK, USE THE BIGGEST!
 In .o, it not consume .bss. But in finally .elf, it contributes to .bss.
2. to make a symbol weak, either define it as a uninitialized global var, or add explicite __attribute__((weak)) to it.
In .o symbol tables, the former is GLOBAL&COM(has no 'WEAK' in st_info, but actually weak, not consume .bss room),
the later is GLOBAL&in .bss. But in a executable elf, both is in .bss and contributes to .bss.
ONCE again, as long as the varibale is weak across multi files: IF ALL ARE WEAK, USE THE BIGGEST!
3.$ arm-none-linux-gnueabi-gcc -fno-common -Wall -c d.c<- -fno-common force an uninitialized global var in .bss and becomes a strong symbol
readelf:
     9: 00000000     8 OBJECT  GLOBAL DEFAULT    3 uninit_var<-[3] is .bss,all uninitilized variable becomes a strong symbol, that cann't be redefined in other files unless it is of weak attribute in other files.
    10: 00000008     8 OBJECT  GLOBAL DEFAULT    3 uninit_var2<--ditto
$ arm-none-linux-gnueabi-gcc -fno-common -Wall d.c tc.c -o a.out
/tmp/ccy5HOEw.o:(.bss+0x0): multiple definition of `uninit_var'<--tc.c&d.c all has a strong symbol uninit_var, no uninit_var2 error as it is weak in tc.c
/tmp/ccSVCiIu.o:(.bss+0x0): first defined here
/home/rednoah/opt/src/mini2440/arm-2008q3/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/../../../../arm-none-linux-gnueabi/bin/ld: Warning: size of symbol `uninit_var' changed from 8 in /tmp/ccSVCiIu.o to 4 in /tmp/ccy5HOEw.o
collect2: ld returned 1 exit status
*/
long long uninit_var;
long long uninit_var2;