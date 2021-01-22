ENTRY(reset)
SECTIONS
{
    . = 0x10000;
    .text : {
        ts.o(.text)
        *(.text)
    }
    .data : {
        *(.data)
    }
    /*if no .test block as follows, section test in ts.s will be put after .bss*/
    .test : {
        *(test)
    }
    .bss : {
        *(.bss)
    }
    /*should at least 4B aligned, ld script ALIGN unit is byte*/
    . = ALIGN(4);
    . += 0x1000;
    stack_top = .;
}