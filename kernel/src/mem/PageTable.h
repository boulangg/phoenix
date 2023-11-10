/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>

#include "Page.h"
#include "PageTableStruct.h"

#include "utils/Elf64File.h"

namespace kernel::mem {

class PageTable
{
public:
    std::uint64_t getPageTablePhysAddr()
    {
        return reinterpret_cast<uint64_t>(&pml4t) - kernel::mem::Page::KERNEL_BASE_LINEAR_MAPPING;
    }

    std::list<Page*> mapPage(MemoryAllocator allocator, uint16_t highLvlFlags, uint64_t virtAddr, uint16_t flags,
                             bool noExec, page_size pageSize, uint64_t physAddr)
    {
        return pml4t->mapPage(allocator, highLvlFlags, virtAddr, flags, noExec, pageSize, physAddr);
    }

    static void initKernelPageTable(PageTable* table, std::size_t hhdm, std::size_t hhdmSize,
                                    std::size_t kernelPhysBase, utils::Elf64File& kernelFile, MemoryAllocator allocator);

private:
    pml4e_t pml4t[512];
};

}
