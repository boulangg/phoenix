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
#define KERNEL_STACK_BOTTOM  0xFFFF840000000000ul
#define KERNEL_STACK_TOP     0xFFFF840000200000ul
#define SYSCALL_STACK_BOTTOM 0xFFFF850000000000ul
#define SYSCALL_STACK_TOP    0xFFFF850000200000ul
