/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "KernelSyscall.h"

#include "Constant.h"
#include "Kernel.h"
#include "utils/Utils.h"

static std::uint64_t kernel_sbrk_curr = KERNEL_BRK_HEAP_START;
static std::uint64_t kernel_max_heap_curr = KERNEL_BRK_HEAP_START;

void* sys_brk(void* addr)
{
    if (addr == 0) {
        return reinterpret_cast<void*>(kernel_sbrk_curr);
    }

    std::uint64_t old_brk = kernel_sbrk_curr;
    std::uint64_t new_brk = reinterpret_cast<std::uint64_t>(addr);
    if ((new_brk < KERNEL_BRK_HEAP_START) || (new_brk > KERNEL_BRK_HEAP_END)) {
        return reinterpret_cast<void*>(old_brk);
    }

    kernel_max_heap_curr = kernel::utils::alignUp(new_brk, PAGE_SIZE);
    kernel_sbrk_curr = new_brk;
    return reinterpret_cast<void*>(kernel_sbrk_curr);
}

// void* sys_mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset)
//{
//
// }
