ENTRY(reset_start)
SECTIONS
{
    . = 0x10000;
    .text : {
        ts.o(.text)
        *(.text) /* if a filename appears explicitly and is also matched by a wildcard pattern, the linker will use
        the 1st matched*/
        *(.rodata)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
        *(COMMON)
    }
    . = ALIGN(8); /*the single operand ALIGN doesn't change the value of the location counter, it just
    does arithmetic on it:ALIGN(8) is equivalent to ALIGN(.,8)*/
    . += 0x100000;
    stack_top = .;
}