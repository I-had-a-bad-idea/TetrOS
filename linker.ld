ENTRY(_start)

SECTIONS
{
	. = 0x1000;

  	/* text section */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text.start) /* entry point */
		*(.text)
	}

	/* read only data section */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* data section */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* bss section */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

	. = ALIGN(16);
	stack_top = .;
	. += 64K; /* Way to much but cant hurt  */
	stack_bottom = .;

}