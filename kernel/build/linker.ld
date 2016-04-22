/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	. = 1K;

	.multiboot BLOCK(4K) : ALIGN(4K)
	{
        _multiboot_start = .;
        *(.multiboot)
        . = ALIGN(0x1000);
        _multiboot_end = .;
    }
    
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 1M;
	
	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	.text BLOCK(4K) : ALIGN(4K)
	{
		_text_start = .;
		*/boot.S.o(.text)
		. = ALIGN(4K);
		*(.text)
		_text_end = ALIGN(4K);
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		_rodata_start = .;
		*(.rodata)
		_rodata_end = .;
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		_data_start = .;
		*(.data)
		_data_end = .;
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		_bss_start = .;
		*(.bootstrap_stack)
		*(COMMON)
		*(.bss)
		_bss_end = .;
	}
	
	_end = .;
	
	/* The memory map */
	idt = 0x1000;
	pml4t = 0x2000;
	pdpt = 0x3000;
	pdt = 0x4000;
	pt = 0x5000;
	gdt = 0x10000;
	tss = 0x20000;
	
	/* Beginning of kernel memory heap */
	mem_heap = _end;

	/* End of kernel memory heap: 64MB */
    mem_heap_end = 0x4000000;

    /* End of managed memory: 256MB */
	mem_end = 0x10000000;

    /* User space start: 1GB */
    user_start = 0x40000000;
    user_run = user_start;

    /* User space end: 4GB (32 bits kernel) */
    userspace_end = 0xFFFFFFFF;
}
