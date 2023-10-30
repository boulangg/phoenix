/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>
#include <list>

#include "MemoryRegion.h"
#include "PageTable.h"

#include "fs/File.h"
#include "utils/Elf64File.h"

namespace kernel::mem {

class MemoryDescriptor
{
public:
    MemoryDescriptor(PageTable pageTable);
    MemoryDescriptor(const MemoryDescriptor& baseMemDesc);

    void setKernelStack(std::uint64_t physAddr);
    void load(utils::Elf64File& elf64, fs::File* file);
    std::uint64_t mmap(std::uint64_t addr, std::size_t length, std::uint32_t prot, std::uint32_t flags, fs::File* file,
                       off_t offset);

    std::uint64_t updatePageTable(uint16_t highLvlFlags, uint64_t virtAddr, uint16_t flags, bool noExec, Page* page,
                                  std::size_t order)
    {
        for (std::size_t i = 0; i < (1ul << order); ++i) {
            _pageTable.mapPage(highLvlFlags, virtAddr + i * PAGE_SIZE, flags, noExec, page_size::pt_4KB,
                               page->getKernelAddr() + i * PAGE_SIZE);
        }

        return page->getKernelAddr();
    }

    PageTable getPageTable()
    {
        return _pageTable;
    }

    std::uint64_t getPageTablePtr()
    {
        return _pageTable.getPageTablePhysAddr();
    }

private:
    std::list<MemoryRegion> _memoryRegions;
    PageTable _pageTable;
};

}
