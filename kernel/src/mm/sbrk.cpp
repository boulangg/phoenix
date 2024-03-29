/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "sbrk.hpp"

#include <errno.h>
#include <stddef.h>

#include "PageTable.hpp"
#include "PhysicalAllocator.hpp"
#include <include/constant.h>

static char* kernel_sbrk_curr = (char*)KERNEL_HEAP_START;
static char* kernel_max_heap_curr = (char*)KERNEL_HEAP_START;

void* sys_brk(void* addr)
{
    if (addr == 0) {
        void* ret = (void*)kernel_sbrk_curr;
        return ret;
    } else {
        char* old_brk = kernel_sbrk_curr;
        char* new_brk = (char*)addr;
        if ((new_brk < (char*)KERNEL_HEAP_START) || (new_brk > (char*)KERNEL_HEAP_END)) {
            return (void*)old_brk;
        }

        kernel_sbrk_curr = new_brk;

        while (new_brk > kernel_max_heap_curr) {
            Page* new_page = PhysicalAllocator::allocZeroedPage();
            PageTable kernelPageTable = PageTable::getKernelPageTable();
            kernelPageTable.mapPage(new_page->physAddr, (uint64_t*)kernel_max_heap_curr, 0x3, 0x3);
            kernel_max_heap_curr += PAGE_SIZE;
        }
        return kernel_sbrk_curr;
    }
}
