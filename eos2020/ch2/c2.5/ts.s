    .globl reset
reset:
    ldr sp, = stack_top
    bl main
    b .
/*
readelf -a ts.o:
stop is NOTYPE LOCAL while reset is NOTYPE, GLOBAL. as stop is claimed by .globl!
*/
stop:
