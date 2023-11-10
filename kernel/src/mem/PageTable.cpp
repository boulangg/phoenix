/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "PageTable.h"

#include <list>

#include "Constant.h"

namespace kernel::mem {

void PageTable::initKernelPageTable(PageTable* table, std::size_t hhdm, std::size_t pageCount,
                                    std::size_t kernelPhysBase, utils::Elf64File& kernelFile,
                                    MemoryAllocator* allocator)
{
    std::uint16_t hhdmLvlFlags = pt_flag::FLAG_P | pt_flag::FLAG_W;

    // Set Higher Half Direct Mapping using 2M page size (512 * PAGE_SIZE)
    for (size_t i = 0; i < pageCount; i += 512) {
        std::uint64_t physAddr = i * PAGE_SIZE;
        std::uint64_t virtAddr = i * PAGE_SIZE + hhdm;
        table->mapPage(allocator, hhdmLvlFlags, virtAddr, hhdmLvlFlags, false, page_size::pt_2MB, physAddr);
    }

    // Set Kernel Mapping
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

        std::uint64_t physBase = kernel::utils::alignDown(kernelPhysBase + pHdr.p_offset, PAGE_SIZE);
        std::uint64_t virtBase = kernel::utils::alignDown(pHdr.p_vaddr, PAGE_SIZE);
        std::uint64_t virtTop = kernel::utils::alignUp(pHdr.p_vaddr + pHdr.p_memsz, PAGE_SIZE);
        std::size_t pageCount = (virtTop - virtBase) / PAGE_SIZE;

        for (std::size_t i = 0; i < pageCount; ++i) {
            std::size_t physAddr = physBase + i * PAGE_SIZE;
            std::size_t virtAddr = virtBase + i * PAGE_SIZE;
            table->mapPage(allocator, hhdmLvlFlags, virtAddr, flags, noExec, page_size::pt_2KB, physAddr);
        }
    }

    // Set Base for Kernel Heap sbrk (2 MB) + mmap (2 MB)
    Page* brkPage = allocator->allocZeroedPage();
    std::uint64_t brkPagePhysAddr = brkPage->getPhysicalAddr();
    table->mapPage(allocator, hhdmLvlFlags, KERNEL_BRK_HEAP_START, hhdmLvlFlags, false, page_size::pt_2MB,
                   brkPagePhysAddr);
    Page* mmapPage = allocator->allocZeroedPage();
    std::uint64_t mmapPagePhysAddr = mmapPage->getPhysicalAddr();
    table->mapPage(allocator, hhdmLvlFlags, KERNEL_MMAP_HEAP_START, hhdmLvlFlags, false, page_size::pt_2MB,
                   mmapPagePhysAddr);

    // Set Base for Kernel Stack (2MB)
    Page* stackPage = allocator->allocZeroedPage();
    std::uint64_t stackPagePhysAddr = stackPage->getPhysicalAddr();
    table->mapPage(allocator, hhdmLvlFlags, KERNEL_STACK_BOTTOM, hhdmLvlFlags, false, page_size::pt_2MB,
                   stackPagePhysAddr);
}

}
