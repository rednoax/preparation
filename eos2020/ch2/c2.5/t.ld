ENTRY(reset)
SECTIONS
{
    . = 0x10000;
    /*.text . + 4 : {*//*then .text vma will be 0x10004*/
    .text . : {
        *(.text)
        /*no *(.rodata) but final t.bin actually contains .rodata of t.elf, after .text of t.elf*/
    }
    .data : {
        *(.data)
    }
    /*note the following will not be included in t.bin.*/
    .bss : {
        *(.bss)
    }
    . = ALIGN(4);
    . += 0x1000;
    stack_top = .;
}