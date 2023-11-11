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
        return _pageTable.getPageTablePhysAddr();
    }

    std::uint64_t mapPage(MemoryAllocator* allocator, uint16_t highLvlFlags, uint64_t virtAddr, uint16_t flags,
                          bool noExec, page_size pageSize, std::uint64_t physAddr)
    {
        return _pageTable.mapPage(allocator, highLvlFlags, virtAddr, flags, noExec, pageSize, physAddr);
    }

private:
    page_table _pageTable;
};

}

// ASM functions
#ifdef __cplusplus
extern "C"
{
#endif

extern void set_CR3(std::uint64_t pml4t);
extern std::uint64_t get_CR3();

#ifdef __cplusplus
}
#endif
