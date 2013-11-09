/*syntax error:OUTPUT_FORMAT(srec, srec, srec)*/
/*
arm-linux-ld: error: Cannot change output format whilst linking ARM binaries.
OUTPUT_FORMAT(srec)
*/
/*
arm-linux-objdump -i
...
    elf32-littlearm elf32-bigarm elf32-little elf32-big srec 
arm elf32-littlearm elf32-bigarm elf32-little elf32-big srec 

    symbolsrec verilog tekhex binary ihex 
arm symbolsrec verilog tekhex binary ihex 
*/
/*
if remove OUTPUT_FORMAT/OUTPUT_ARCH, or both, nothing will happen on "final"
*/
OUTPUT_FORMAT(elf32-littlearm)
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
	.text :
	{
		*(.text)
	}
	.data :
	{
		*(.data)
	}
	.bss :
	{
		*(.bss)
	}
}

