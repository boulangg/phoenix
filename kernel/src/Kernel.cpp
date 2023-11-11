/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Kernel.h"

#include "Constant.h"
#include "GlobalDescTable.h"
#include "utils/Elf64File.h"

namespace kernel {

mem::MemoryAllocator Kernel::_memory;
mem::AddressSpace* Kernel::_kernelAddressSpace __attribute__((aligned(4096)));
core::InterruptDispatcher Kernel::_interrupt;

typedef void (*func_ptr)(void);
extern "C" func_ptr _init_array_start[0], _init_array_end[0];

void Kernel::setupGlobalConstructors()
{
    for (auto fn = _init_array_start; fn != _init_array_end; fn++) {
        (*fn)();
    }
}

static mem::AddressSpace* initKernelPageTable(std::size_t hhdm, std::size_t pageCount,
                                std::size_t kernelPhysBase, utils::Elf64File& kernelFile,
                                mem::MemoryAllocator* allocator)
{
    mem::PageTable table{};
    std::uint16_t hhdmLvlFlags = mem::pt_flag::FLAG_P | mem::pt_flag::FLAG_W;

    // Set Higher Half Direct Mapping using 2M page size (512 * PAGE_SIZE)
    for (size_t i = 0; i < pageCount; i += 512) {
        std::uint64_t physAddr = i * PAGE_SIZE;
        std::uint64_t virtAddr = i * PAGE_SIZE + hhdm;
        table.mapPage(allocator, hhdmLvlFlags, virtAddr, hhdmLvlFlags, false, mem::page_size::pt_2MB, physAddr);
    }

    // Set Kernel Mapping
    for (const auto& pHdr : kernelFile.getProgramHeaders()) {
        bool noExec = true;
        std::uint16_t flags = mem::pt_flag::FLAG_P;
        if (pHdr.p_type != kernel::utils::Elf64::ProgramType::PT_LOAD) {
            continue;
        }

        if (pHdr.p_flags & kernel::utils::Elf64::ProgramFlag::PF_W) {
            flags |= mem::pt_flag::FLAG_W;
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
            table.mapPage(allocator, hhdmLvlFlags, virtAddr, flags, noExec, mem::page_size::pt_2KB, physAddr);
        }
    }

    // Set Base for Kernel Heap sbrk (2 MB) + mmap (2 MB)
    mem::Page* brkPage = allocator->allocZeroedPages(9);
    std::uint64_t brkPagePhysAddr = brkPage->getPhysicalAddr();
    table.mapPage(allocator, hhdmLvlFlags, KERNEL_BRK_HEAP_START, hhdmLvlFlags, false, mem::page_size::pt_2MB,
                   brkPagePhysAddr);
    // mem::Page* mmapPage = allocator->allocZeroedPages(9);
    // std::uint64_t mmapPagePhysAddr = mmapPage->getPhysicalAddr();
    // table->mapPage(allocator, hhdmLvlFlags, KERNEL_MMAP_HEAP_START, hhdmLvlFlags, false, mem::page_size::pt_2MB,
    //                mmapPagePhysAddr);

    // Set Base for Kernel Stack (2MB)
    // Page* stackPage = allocator->allocZeroedPages(9);
    // std::uint64_t stackPagePhysAddr = stackPage->getPhysicalAddr();
    // table->mapPage(allocator, hhdmLvlFlags, KERNEL_STACK_TOP - PAGE_SIZE * 512, hhdmLvlFlags, false,
    // page_size::pt_2MB, stackPagePhysAddr);

    set_CR3(table.getPageTablePhysAddr());

    mem::AddressSpace* kernelAddrSpace = new mem::AddressSpace(table);
    kernelAddrSpace->load(kernelFile, nullptr);

    // Set Base for Kernel Heap sbrk (2 MB)
    std::uint32_t kernelHeapProt =
        mem::MemoryRegion::Prot::READ | mem::MemoryRegion::Prot::WRITE;
    std::uint32_t kernelHeapFlags = 
        mem::MemoryRegion::Flags::PRIVATE | mem::MemoryRegion::Flags::ANON;
    kernelAddrSpace->mmap(KERNEL_BRK_HEAP_START, KERNEL_BRK_HEAP_END - KERNEL_BRK_HEAP_START, kernelHeapProt,
                          kernelHeapFlags, nullptr, 0);

    return kernelAddrSpace;
}

void Kernel::init(KernelInfo& info)
{
    utils::Elf64File kernelFile(info.kernelFileAddr);

    // Generic CPU struct
    GDT::setupGDT();
    TSS::setupTSS();

    // Memory Allocator
    _memory.init(info.pageArray, info.pageCount);

    // Kernel base Page Table
    _kernelAddressSpace = initKernelPageTable(info.hhdm, info.pageCount, info.kernelPhysBase, kernelFile, &_memory);

    // Interrupts
    _interrupt.init();

    //_device.init();
    //_scheduler.init();
}

}
