ENTRY(reset)
SECTIONS
{
    . = 0x10000;
    /*.text . + 4 : {*//*then .text vma will be 0x10004*/
    .text . : {
        *(.text)
        *(.rodata)/*if no *(.rodata), final t.bin still contains .rodata of t.elf, right after extracted
        .text from t.elf; to make it more clear, *(.rodata) is explicitely put here*/
    }
    .data : {
        *(.data)
    }
    /*note the following will not be included in t.bin.*/
    .bss : {
        *(.bss)
/*even no *(COMMON) like below, g_uninit in t.c is still in t.elf's .bss. But to make it more clear, *(COMMON)
is put here!
g_uninit in t.o is COMMON symbol so it doesn't contribute to the size of t.o's .bss. In object file formats
common symbols don't have a particular input section.The linker treats common symbols as though they are in an
input section named "COMMON".
You may use file names with the 'COMMON' section just as with any other input sections.
In most cases, common symbols in input files will be placed in the '.bss' section in the output file.
*/
        *(COMMON)
    }
    . = ALIGN(4);
    . += 0x1000;
    stack_top = .;
}