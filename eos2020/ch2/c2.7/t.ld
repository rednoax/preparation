ENTRY(reset_start)
SECTIONS
{
    . = 0x10000;
    .text : {ts.o *(.text)}
    .data : {*(.data)}
    .bss : {*(.bss)}
    . = ALIGN(8);
    . += 0x100000;
    stack_top = .;
}