ENTRY(start)
SECTIONS
{
    . = 0x10000;/*for qemu*/
    .text : {
        /*ts.o(.text)*/
        *(.text)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
    }
    . = ALIGN(4);
    . += 0x1000;
    stack_top = .;
}