/* Bootloader entry point. */
ENTRY(_start)

SECTIONS
{
	. = 1K;

	/* Multiboot header section */
	.multiboot BLOCK(4K) : ALIGN(4K)
	{
        _multiboot_start = .;
        *(.multiboot)
        . = ALIGN(0x1000);
        _multiboot_end = .;
    }
    
	. = 1M;
	
	/* Text section */
	.text BLOCK(4K) : ALIGN(4K)
	{
		_text_start = .;
		*/boot.S.o(.text)
		. = ALIGN(4K);
		*(.text)
		_text_end = ALIGN(4K);
	}

	/* Read-only data section */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		_rodata_start = .;
		*(.rodata)
		_rodata_end = .;
	}

	/* Read-write data (initialized) section */
	.data BLOCK(4K) : ALIGN(4K)
	{
		_data_start = .;
		*(.data)
		_data_end = .;
	}

	/* Read-write data (uninitialized) and bootstrap stack sections */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		_bss_start = .;
		*(.bootstrap_stack)
		*(COMMON)
		*(.bss)
		_bss_end = .;
	}
	
	_end = .;
	
	/* The default memory map for some cpu specific data structures */
	idt = 0x1000;
	pml4t = 0x2000;
	pdpt = 0x3000;
	pdt = 0x4000;
	pt = 0x5000;
	gdt = 0x10000;
	tss = 0x20000;
	
	/* Beginning of kernel memory heap */
	kernel_mem_heap = _end;

	/* End of kernel memory heap: 64MB */
    kernel_mem_heap_end = 0x4000000;
}
