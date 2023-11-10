/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <list>

#include "MemoryRegion.h"
#include "PageTable.h"

#include "utils/Elf64File.h"

namespace kernel::mem {

class AddressSpace
{
public:
    AddressSpace()
    {
        // Copy kernel pagetable

        // Define Kernel Stack

        // Define Interrupt Stack
    }

    void load(utils::Elf64File& file)
    {
        for (auto& pHdr : file.getProgramHeaders()) {
            bool noExec = true;
            std::uint16_t flags = pt_flag::FLAG_P;
            if (pHdr.p_type != utils::Elf64::ProgramType::PT_LOAD) {
                continue;
            }

            if (pHdr.p_flags & utils::Elf64::ProgramFlag::PF_W) {
                flags &= pt_flag::FLAG_W;
            }
            if (pHdr.p_flags & utils::Elf64::ProgramFlag::PF_X) {
                noExec = false;
            }
        }

        // Define Code segment

        // Define Data segment

        // Define User Heap

        // Define User Stack
    }

private:
    std::list<MemoryRegion> _memoryRegions;
    PageTable _pageTable;
};

}
