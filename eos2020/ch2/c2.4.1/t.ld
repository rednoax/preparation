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
    /*the following stuff will not be included in t.bin.
    readelf -a show .text/.data size in t.elf, their sum==t.bin's bytes size*/
    .bss : {
        *(.bss)
    }
    . = ALIGN(4);
    . += 0x1000;
    stack_top = .;
}
