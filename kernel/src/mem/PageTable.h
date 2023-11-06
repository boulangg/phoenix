/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>

#include "Page.h"
#include "PageTableStruct.h"

namespace kernel::mem {

class PageTable
{
public:
    std::uint64_t getPageTablePhysAddr()
    {
        return (uint64_t)(&pml4t) & ~(kernel::mem::Page::KERNEL_BASE_LINEAR_MAPPING);
    }

    std::list<Page*> mapPage(MemoryAllocator allocator, uint16_t highLvlFlags, uint64_t virtAddr, uint16_t flags,
                             bool noExec, page_size pageSize, uint64_t physAddr)
    {
        return pml4t->mapPage(allocator, highLvlFlags, virtAddr, flags, noExec, pageSize, physAddr);
    }

    static void initKernelPageTable(MemoryAllocator allocator, std::size_t hhdm, std::size_t hhdm_size,
                                    std::size_t kernel_phys_addr, std::size_t kernel_virt_addr);

private:
    pml4e_t pml4t[512];

    static PageTable _kernelPageTable __attribute__((aligned(4096)));
};

}
