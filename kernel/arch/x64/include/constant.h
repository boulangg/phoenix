#ifndef __ARCH_CONSTANT_H__
#define __ARCH_CONSTANT_H__

#define KERNEL_MULTIBOOT		0x00001000
#define KERNEL_BOOT_VMA			0x00100000
#define KERNEL_HIGH_VMA			0xFFFFFFFFC0000000
#define KERNEL_PHYSICAL_START	0x01000000

#if !defined(LINKER_FILE) && !defined(ASM_FILE)
#include <stdint.h>

extern "C" uint64_t _kernel_code_start[];
extern "C" uint64_t _kernel_code_end[];
extern "C" uint64_t _kernel_data_start[];
extern "C" uint64_t _kernel_data_end[];

#define USER_START		((uint64_t)0x0)
#define USER_END		((uint64_t)0x)

#define KERNEL_START			((uint64_t)0xFFFF800000000000)
#define KERNEL_END				((uint64_t)0xFFFFFFFFFFFFFFFF)

// Mapping space up to 1 To of RAM
#define KERNEL_MAPPING_START	((uint64_t)0xFFFF800000000000)
#define KERNEL_MAPPING_END		((uint64_t)0xFFFF810000000000)

// Kernel malloc space up to 1 Go
#define KERNEL_HEAP_START		((uint64_t)0xFFFF820000000000)
#define KERNEL_HEAP_END		    ((uint64_t)0xFFFF820040000000)

// Default kernel stack of 2 Mo
#define KERNEL_STACK_START		((uint64_t)0xFFFF840000000000)
#define KERNEL_STACK_END	    ((uint64_t)0xFFFF840000200000)

#define KERNEL_CODE_START		((uint64_t)_kernel_code_start)
#define KERNEL_CODE_END			((uint64_t)_kernel_code_end)
#define KERNEL_DATA_START		((uint64_t)_kernel_data_start)
#define KERNEL_DATA_END			((uint64_t)_kernel_data_end)

#endif

#endif // __ARCH_CONSTANT_H__
