/*If no ENTRY(start), "readelf -a" shows:
Entry point address:               0x0
ld manual "3.4.1 Setting the Entry Point" shows the sequence of entry point setting sequence
*/
/*ENTRY(start)*/
/*
https://stackoverflow.com/questions/39269613/syntax-error-in-linker-script
I believe the issue is lack of space between ".text" and the colon.
So use: '.text<space>:' rather than '.text:'
*/
SECTIONS
{
	. = 0x10000;/*can be ignored for program in ts.s is PositionIndependantCode*/
	.text : {
		*(.text)
	}
	.data : {
		*(.data)
	}
	.bss : {
		*(.bss)
	}
/*
$ arm-none-eabi-readelf -a ts.elf
shows ".full" section size is 4B only, if I don't define section full in ts.s, ie no ".section full" in ts.s, there will be no ANY info about ".full"
section in output of "readelf -a";
So once *(.text) *(data) is extracted before the following block, then they can't be extracted again!
*/
	.full : {
		*(.text)
		*(.data)
		*(full)
	}
}