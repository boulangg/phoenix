/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

/* Bootloader entry point. */
ENTRY(_start)

KERNEL_MULTIBOOT = 0x00001000;
KERNEL_BOOT_VMA = 0x00100000;
KERNEL_HIGH_VMA = 0xFFFFFFFFC0000000;

SECTIONS
{
	. = KERNEL_MULTIBOOT;

	/* Multiboot header section */
	.multiboot BLOCK(4K) : ALIGN(4K)
	{
        _multiboot_start = .;
        *(.multiboot)
        . = ALIGN(0x1000);
        _multiboot_end = .;
    }
    
	. = KERNEL_BOOT_VMA;
	
	/* Low mem section */
	.low_mem BLOCK(4K) : ALIGN(4K)
	{
		_low_mem_start = .;
		. = ALIGN(4K);
		*/boot.S.o(.text)
		*/long_mode.S.o(.text)
		_low_mem_end = ALIGN(4K);
	}
	
	.bootstrap_stack BLOCK(4K) : ALIGN(4K)
	{
		*(.bootstrap_stack)
	}
	
	.low_mem_data BLOCK(4K) : ALIGN(4K)
	{
		*/boot.S.o(.data)
		*/long_mode.S.o(.data)
	}
	
	
	. += KERNEL_HIGH_VMA;
	
	/* High mem section */
	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_HIGH_VMA)
	{
		_text_start = .;
		*(.text)
		_text_end = ALIGN(4K);
	}

	/* Read-only data section */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_HIGH_VMA)
	{
		_rodata_start = .;
		*(.rodata)
		_rodata_end = .;
	}

	/* Read-write data (initialized) section */
	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_HIGH_VMA)
	{
		_data_start = .;
		*(.data)
		_data_end = .;
	}

	/* Read-write data (uninitialized) and bootstrap stack sections */
	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_HIGH_VMA)
	{
		_bss_start = .;
		*(COMMON)
		*(.bss)
		_bss_end = .;
	}
	
	_end = .;
	
	/* Beginning of kernel memory heap */
	_kernel_mem_heap = _end;
	/* End of kernel memory heap: 64MB */
    _kernel_mem_heap_end = KERNEL_HIGH_VMA + 0x4000000;
}
