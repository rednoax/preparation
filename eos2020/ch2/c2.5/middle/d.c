/*
arm-none-linux-gnueabi-gcc -Wall -c tc.c
arm-none-linux-gnueabi-gcc -Wall -c d.c

arm-none-linux-gnueabi-readelf -a d.o|less

     8: 00000008     8 OBJECT  GLOBAL DEFAULT  COM uninit_var<--weak in COMMON, 8B for long long
     9: 00000008     8 OBJECT  GLOBAL DEFAULT  COM uninit_var2<--ditto

arm-none-linux-gnueabi-readelf -a tc.o|less

   26: 00000004     4 OBJECT  GLOBAL DEFAULT  COM uninit_var<---weak in COMMON, 4B for int

so all uninitialized global var in d.c and tc.c are all "in COM block"!
arm-none-linux-gnueabi-gcc -Wall tc.c d.c -o a.out  <--no err as uninit global var is WEAK!

arm-none-linux-gnueabi-readelf -a a.out|less

   113: 00010c88     8 OBJECT  GLOBAL DEFAULT   24 uninit_var<---8B in .bss([24] is .bss)
   114: 00010c80     0 NOTYPE  GLOBAL DEFAULT   23 _edata
   115: 00008a04     0 FUNC    GLOBAL DEFAULT   13 _fini
   116: 00010c98     0 NOTYPE  GLOBAL DEFAULT   24 __bss_end__
   117: 00008518    36 FUNC    GLOBAL DEFAULT   12 uputc
   118: 00010c78     0 NOTYPE  GLOBAL DEFAULT   23 __data_start
   119: 00010c90     8 OBJECT  GLOBAL DEFAULT   24 uninit_var2<---ditto

uninit_var is uninitialized global var in tc.c and d.c. Both of uninit_var is weak!
IF ALL ARE WEAK, USE THE BIGGEST!
*/
long long uninit_var;
long long uninit_var2;