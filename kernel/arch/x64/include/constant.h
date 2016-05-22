/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __ARCH_CONSTANT_H__
#define __ARCH_CONSTANT_H__

#define KERNEL_MULTIBOOT		0x00001000
#define KERNEL_BOOT_VMA			0x00100000
#define KERNEL_HIGH_VMA			0xFFFFFFFFC0000000
#define KERNEL_PHYSICAL_START	0x01000000

#define PAGE_SIZE				0x1000
#define PAGE_ADDR_MASK			(~(PAGE_SIZE-1))

#define EARLY_FULL_MAPPING		0x04000000

#define PAGE_TABLE_ENTRY_MASK 	0x3FF;
#define PML4E_INDEX_OFFSET		39
#define PML4E_INDEX_MASK				0x1FF;
#define PDPE_INDEX_OFFSET		30
#define PDPE_INDEX_MASK				0x1FF;
#define PDE_INDEX_OFFSET		21
#define PDE_INDEX_MASK				0x1FF;
#define PTE_INDEX_OFFSET		12
#define PTE_INDEX_MASK				0x1FF;

// Kernel malloc space up to 1 Go
#define KERNEL_HEAP_START		0xFFFF820000000000
#define KERNEL_HEAP_END		    0xFFFF820040000000

// Default kernel stack of 64 Ko
#define KERNEL_STACK_BOTTOM		0xFFFF840000000000
#define KERNEL_STACK_TOP	    0xFFFF840000010000

// Default interrupt stack of 8 Ko
#define KERNEL_IST1_BOTTOM		0xFFFF860000000000
#define KERNEL_IST1_TOP	    	0xFFFF860000002000

#if !defined(LINKER_FILE) && !defined(ASM_FILE)
#include <stdint.h>

extern "C" uint64_t _kernel_code_start[];
extern "C" uint64_t _kernel_code_end[];
extern "C" uint64_t _kernel_data_start[];
extern "C" uint64_t _kernel_data_end[];

#define USER_START		((uint64_t)0x0000000000000000)
#define USER_END		((uint64_t)0x0000800000000000)

#define KERNEL_START			((uint64_t)0xFFFF800000000000)
#define KERNEL_END				((uint64_t)0xFFFFFFFFFFFFFFFF)

// Mapping space up to 1 To of RAM
#define KERNEL_MAPPING_START	((uint64_t)0xFFFF800000000000)
//#define KERNEL_MAPPING_END		((uint64_t)0xFFFF810000000000)

#define KERNEL_CODE_START		((uint64_t)_kernel_code_start)
#define KERNEL_CODE_END			((uint64_t)_kernel_code_end)
#define KERNEL_DATA_START		((uint64_t)_kernel_data_start)
#define KERNEL_DATA_END			((uint64_t)_kernel_data_end)

#endif

#endif // __ARCH_CONSTANT_H__
