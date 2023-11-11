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

class AddressSpace
{
public:
    AddressSpace(PageTable pageTable);
    AddressSpace(const AddressSpace* kernelAddressSpace);

    void load(utils::Elf64File& elf64, fs::File* file);
    std::uint64_t mmap(std::uint64_t addr, std::size_t length, std::uint32_t prot, std::uint32_t flags, fs::File* file,
                       off_t offset);

private:

    std::list<MemoryRegion> _memoryRegions;
    PageTable _pageTable;
};

}
