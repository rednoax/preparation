/*this script is from c2.6 org source, which specified only ts.o t.o, but no panda.o porkcar.o in ld's cmd line
ld manual said where filename comes from:
a. cmd line of ld, eg ts.o t.o here
b. INPUT command in ld script file. They way is seldom used. Both u-boot and kernel didn't use it
c. the file name in script eg panda.o and portcar.o here: "when you use a file name which is not an 'archive:file'
specifier and does not contain any wild card chars, the link will first see if you also specified the file name on
the cmd line or in INPUT command. If you did not, the linker will atempt to open the file as an input file, as though
it appeared on the cmd line";
this way is also not standard. It is only used in this poor.ld. Both u-boot and kernel didn't use it.
*/
ENTRY(reset_start)
SECTIONS
{
    . = 0x10000;
    .text : {
        ts.o(.text)
        *(.text)
        *(.rodata)
    }
    .data : {
        *(.data)
/*if there is a x.o in cur dir, it is not used at all for it is not among the above a to c scope.
LD MANUAL: File name wildcard patterns only match files which are explicitly specified on the command
line or in an INPUT command. The linker does not search directories to expand wildcards.*/
    }
    .bss : {
        *(.bss)
        *(COMMON)
    }
/*
*(.data) has extracted panda.o and porkcar.o's .data, ./poor.sh call ld -verbose shows:
attempt to open ts.o succeeded
ts.o
attempt to open panda.o succeeded
panda.o
attempt to open porkcar.o succeeded
porkcar.o
attempt to open t.o succeeded<--WHY t.o is after panda.o and porkcar.o, even t.o is from cmd line?
t.o
*/
/*
If a file name matches more than one wildcard pattern, or if a file name appears explicitly
and is also matched by a wildcard pattern, the linker will use the first match in the linker
script. 
so at last you will not see .bss in outputed elf or bin file as panda.o porkcar.o has been extracted before
arriving here!
*/
    .data : {
        panda.o /*these 2 files MUST be specified here as they're not specified in the ld's cmd line*/
        porkcar.o /*if panda.o and porkcar.o is swapped, their opened sequence in ld -verbose swaps too*/
    }
    . = ALIGN(8);
    . += 0x100000;
    stack_top = .;
}