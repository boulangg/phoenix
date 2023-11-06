/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "PageTable.h"

#include <list>

#include "fs/Elf64File.h"

namespace kernel::mem {

page_table kernel_pml4t __attribute__((aligned(4096)));

PageTable PageTable::_kernelPageTable;

void PageTable::initKernelPageTable(MemoryAllocator allocator, std::size_t hhdm, std::size_t hhdm_size,
                                    std::size_t kernel_phys_addr, std::size_t kernel_virt_addr)
{
    std::uint16_t hhdmLvlFlags = pt_flag::FLAG_P | pt_flag::FLAG_W;

    std::list<Page*> allPages;

    // Set Higher Half Direct Mapping using 2M page size (512 * PAGE_SIZE)
    for (size_t i = 0; i < hhdm_size; i += 512) {
        std::uint64_t physAddr = i * PAGE_SIZE;
        std::uint64_t virtAddr = i * PAGE_SIZE + hhdm;
        auto pages = PageTable::_kernelPageTable.mapPage(allocator, hhdmLvlFlags, virtAddr, hhdmLvlFlags, false,
                                                         page_size::pt_2MiB, physAddr);
        allPages.insert(allPages.begin(), pages.begin(), pages.end());
    }

    // Set Kernel Mapping
    kernel::fs::Elf64File kernelFile(kernel_virt_addr);
    for (const auto& pHdr : kernelFile.getProgramHeaders()) {
        bool noExec = true;
        std::uint16_t flags = pt_flag::FLAG_P;
        if (pHdr.p_type != kernel::utils::Elf64::ProgramType::PT_LOAD) {
            continue;
        }

        if (pHdr.p_flags & kernel::utils::Elf64::ProgramFlag::PF_W) {
            flags &= pt_flag::FLAG_W;
        }
        if (pHdr.p_flags & kernel::utils::Elf64::ProgramFlag::PF_X) {
            noExec = false;
        }

        std::uint64_t physBase = kernel::utils::alignDown(kernel_phys_addr + pHdr.p_offset, PAGE_SIZE);
        std::uint64_t virtBase = kernel::utils::alignDown(pHdr.p_vaddr, PAGE_SIZE);
        std::uint64_t virtTop = kernel::utils::alignUp(pHdr.p_vaddr + pHdr.p_memsz, PAGE_SIZE);
        std::size_t pageCount = (virtTop - virtBase) / PAGE_SIZE;
        
        for (std::size_t i = 0; i < pageCount; ++i) {
            std::size_t physAddr = physBase + i * PAGE_SIZE;
            std::size_t virtAddr = physBase + i * PAGE_SIZE; 
            auto pages = PageTable::_kernelPageTable.mapPage(allocator, hhdmLvlFlags, virtAddr, flags, noExec,
                                                             page_size::pt_2KiB, physAddr);
            allPages.insert(allPages.begin(), pages.begin(), pages.end());
        }
    
    }

}

}
