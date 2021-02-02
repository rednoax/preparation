ENTRY(reset_start)
SECTIONS
{
    . = 0x1000;
    .text : {
        *(.text)
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