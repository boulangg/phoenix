/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __ARCH_CONSTANT_H__
#define __ARCH_CONSTANT_H__

#define KERNEL_MULTIBOOT      0x00001000
#define KERNEL_BOOT_VMA       0x00100000
#define KERNEL_HIGH_VMA       0xFFFFFFFFC0000000
#define KERNEL_PHYSICAL_START 0x01000000

#define PAGE_SIZE      0x1000
#define PAGE_ADDR_MASK (~(PAGE_SIZE - 1))

#define EARLY_FULL_MAPPING 0x04000000

#define PAGE_TABLE_ENTRY_MASK 0x3FF;
#define PML4E_INDEX_OFFSET    39
#define PML4E_INDEX_MASK      0x1FF;
#define PDPE_INDEX_OFFSET     30
#define PDPE_INDEX_MASK       0x1FF;
#define PDE_INDEX_OFFSET      21
#define PDE_INDEX_MASK        0x1FF;
#define PTE_INDEX_OFFSET      12
#define PTE_INDEX_MASK        0x1FF;

#define RFLAGS_INIT (1 << 9)

#define USER_START 0x0000000000000000
#define USER_END   0x0000800000000000

// User malloc space
#define USER_HEAP_START 0x0000000002000000
#define USER_HEAP_END   0x0000000002010000

// User stack space
#define USER_STACK_START 0x0000007fffff0000
#define USER_STACK_END   0x0000008000000000

// User mmap space
#define USER_MMAP_START 0x0000008000000000
#define USER_MMAP_END   0x00007ffffff00000

// User interrupt stack space
#define USER_INT_STACK_START 0x00007ffffffd0000
#define USER_INT_STACK_END   0x00007ffffffd2000

// User syscall stack space
#define USER_SYSCALL_STACK_START 0x00007fffffff0000
#define USER_SYSCALL_STACK_END   0x00007fffffff2000

// Kernel space
#define KERNEL_START 0xFFFF800000000000
#define KERNEL_END   0xFFFFFFFFFFFFFFFF

// Mapping phhysical space up to 1 To of RAM
#define KERNEL_MAPPING_START 0xFFFF800000000000
#define KERNEL_MAPPING_END   0xFFFF810000000000

// Kernel malloc space up to 1 Go
#define KERNEL_HEAP_START 0xFFFF820000000000
#define KERNEL_HEAP_END   0xFFFF820040000000

// Default kernel stack of 64 Ko
#define KERNEL_STACK_BOTTOM 0xFFFF840000000000
#define KERNEL_STACK_TOP    0xFFFF840000010000

// Default interrupt stack of 8 Ko
#define KERNEL_IST1_BOTTOM 0xFFFF860000000000
#define KERNEL_IST1_TOP    0xFFFF860000002000

// Default syscall stack of 8 Ko
#define KERNEL_SYSCALL_BOTTOM 0xFFFF860000004000
#define KERNEL_SYSCALL_TOP    0xFFFF860000006000

#if !defined(LINKER_FILE) && !defined(ASM_FILE)
#include <stdint.h>

extern "C" uint64_t _kernel_code_start[];
extern "C" uint64_t _kernel_code_end[];
extern "C" uint64_t _kernel_data_start[];
extern "C" uint64_t _kernel_data_end[];

#define KERNEL_CODE_START _kernel_code_start
#define KERNEL_CODE_END   _kernel_code_end
#define KERNEL_DATA_START _kernel_data_start
#define KERNEL_DATA_END   _kernel_data_end

extern "C" uint64_t _initrd_start[];
extern "C" uint64_t _initrd_end[];

#define INITRD_START _initrd_start
#define INITRD_END   _initrd_end

/* IDT and GDT constant */
// Size Bits
#define SZ_A 0x1
#define SZ_L 0x2
#define SZ_D 0x4
#define SZ_G 0x8

// Common flags
#define FLAG_P    0x80
#define FLAG_DPL0 0x00
#define FLAG_DPL1 0x20
#define FLAG_DPL2 0x40
#define FLAG_DPL3 0x60

// Code flags
#define FLAG_A    0x01
#define FLAG_R    0x02
#define FLAG_C    0x04
#define FLAG_CODE 0x18

// Data flags
#define FLAG_W    0x02
#define FLAG_E    0x04
#define FLAG_DATA 0x10

// System Descriptor Type Flags
#define FLAG_LDT     0x02
#define FLAG_TSS_AVL 0x09
#define FLAG_TSS_BSY 0x0B
#define FLAG_CALL    0x0C
#define FLAG_INT     0x0E
#define FLAG_TRAP    0x0F

// Selector index
#define SEL_NULL       0x00
#define SEL_KERNEL_CS  0x10
#define SEL_KERNEL_DS  0x18
#define SEL_TSS        0x20
#define SEL_USER_CS_32 0x43
#define SEL_USER_DS_32 0x4A
#define SEL_USER_CS    0x53
#define SEL_USER_DS    0x5A

// TSS Infos
#define TSS_INDEX (SEL_TSS >> 3)
#define TSS_BASE  ((uint64_t)&tss)
#define TSS_LIMIT sizeof(tss)
#define TSS_FLAGS (FLAG_DPL0 | FLAG_TSS_AVL)
#define TSS_SIZEB 0

// USER_CS Infos
#define USER_CS_INDEX (SEL_USER_CS >> 3)
#define USER_CS_BASE  0x00 // ignored
#define USER_CS_LIMIT 0x00 // ignored
#define USER_CS_FLAGS (FLAG_DPL3 | FLAG_CODE)
#define USER_CS_SIZEB SZ_L

// USER_DS Infos
#define USER_DS_INDEX (SEL_USER_DS >> 3)
#define USER_DS_BASE  0x00 // ignored
#define USER_DS_LIMIT 0x00 // ignored
#define USER_DS_FLAGS (FLAG_DPL3 | FLAG_DATA | FLAG_W)
#define USER_DS_SIZEB 0x00 // ignored

// USER_CS_32 Infos
#define USER_CS_32_INDEX (SEL_USER_CS_32 >> 3)
#define USER_CS_32_BASE  0x00
#define USER_CS_32_LIMIT (uint32_t)(-1)
#define USER_CS_32_FLAGS (FLAG_DPL3 | FLAG_CODE)
#define USER_CS_32_SIZEB SZ_D

// USER_DS_32 Infos
#define USER_DS_32_INDEX (SEL_USER_DS_32 >> 3)
#define USER_DS_32_BASE  0x00
#define USER_DS_32_LIMIT (uint32_t)(-1)
#define USER_DS_32_FLAGS (FLAG_DPL3 | FLAG_DATA | FLAG_W)
#define USER_DS_32_SIZEB SZ_D

#endif

#endif // __ARCH_CONSTANT_H__
