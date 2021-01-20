ENTRY(start)
SECTIONS {
	. = 0x10000;
	.text : {
		*(.text)
	}
	.data : {
		*(.data)
	}
	.bss : {
		*(.bss)
	}
	. = ALIGN(8);
	/*u can't use the following as nm show: 00011080 A //stack_top
	//stack_top = . + 0x1000;
	*/
	. += 0x1000;
	stack_top = .;
	
}