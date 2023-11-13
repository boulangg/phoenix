/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

// Kernel constant
#define KERNEL_BASE_LINEAR_MAPPING 0xFFFF800000000000ul
#define PAGE_SIZE                  0x1000ul

// Kernel malloc space up to 1 GB (brk) + 1 GB (mmap)
#define KERNEL_BRK_HEAP_START 0xFFFF820000000000ul
#define KERNEL_BRK_HEAP_END   0xFFFF820040000000ul
// #define KERNEL_MMAP_HEAP_START 0xFFFF830000000000ul
// #define KERNEL_MMAP_HEAP_END   0xFFFF830040000000ul

// Default kernel stack of 2MB
#define RESERVED_KERNEL_PAGE_TABLE_LVL4  509ul
#define RESERVED_KERNEL_PAGE_TABLE_START (RESERVED_KERNEL_PAGE_TABLE_LVL4 << 39)
#define KERNEL_STACK_BOTTOM              (RESERVED_KERNEL_PAGE_TABLE_START)
#define KERNEL_STACK_SIZE                (1ul << 21) // 2MB
#define KERNEL_STACK_TOP                 (KERNEL_STACK_BOTTOM + KERNEL_STACK_SIZE)
#define SYSCALL_STACK_BOTTOM             (RESERVED_KERNEL_PAGE_TABLE_START + (1 << 30))
#define SYSCALL_STACK_SIZE               (1ul < 21) // 2MB
#define SYSCALL_STACK_TOP                (SYSCALL_STACK_BOTTOM + SYSCALL_STACK_SIZE - 8)

// User stack space (1MB)
#define USER_STACK_SIZE   (1ul < 20)
#define USER_STACK_TOP    0x0000008000000000
#define USER_STACK_BOTTOM (USER_STACK_TOP - USER_STACK_SIZE)
