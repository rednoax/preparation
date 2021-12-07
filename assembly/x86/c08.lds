ENTRY(start)
SECTIONS
{
    /*. = 0x10000;*//*wrong way*/
    . = 0;
    .text.head ALIGN(16) : {
        section.header.start = .;
        *(header)
    }
    .text ALIGN(16) : {
        section.code_1.start = .;
        *(code_1)
        . = ALIGN(16);
        section.code_2.start = .;
        *(code_2)
    }
    .data ALIGN(16) : {
        section.data_1.start = .;
        *(data_1)
        . = ALIGN(16);
        section.data_2.start = .;
        *(data_2)
    }
    /*stack ALIGN(16): {*/
    stack ALIGN(16) (NOLOAD): {
        section.stack.start = .;
        *(stack)
    }
    trail ALIGN(16) (NOLOAD): {
        section.trail.start = .;
        *(trail)
    }
}
/*
1.'(NOLOAD)' in .lds determins exec-elf's section "stack"'s type: NOBITS if set and PROGBITS is unset;
2.'nobits' in .S determins relocatable elf's section "stack"'s type: ditto.
take stack section for example:
if 1 set but 2 unset: relocatable-elf's 'stack' section is PROGBITS and:
a. if section stack in .S specify "aw": exec-elf has no 'stack' section and it is NOBITS;
b. if no "aw" in a: exec elf contains a 'stack' section and it is PROGBITS.
if 1 unset but 2 set, both relocatable/exec elf stack section is NOBITS.
If stack section is of NOTBITS type in any type elf: the elf file won't contains the section, just like
it will not contain .bss section, so the file size is smalled compared with elf whose stack section
being 'PROGBITS'.
*/