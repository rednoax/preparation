ENTRY(reset)
SECTIONS
{
    . = 0x10000;
    .text : {
        ts.o
        *(.text)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
    }
    . = ALIGN(8); /*ALIGN(align) doesnot change the location pointer but return its next 8B aligned boundary*/
    . += 0x4000;
    svc_stack = .;
    . += 0x4000;
    irq_stack = .;
    dummy0 = . + 0x30;
    dummy1 = . + 0x30;/*dummy0 == dummy1?*/
}