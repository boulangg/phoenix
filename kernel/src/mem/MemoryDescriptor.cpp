/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "MemoryDescriptor.h"

#include <bit>
#include <errno.h>

namespace kernel::mem {

static void initKernelStack(PageTable& pageTable, std::uint64_t physAddr)
{
    // Define Kernel Stack
    std::uint16_t flags = mem::pt_flag::FLAG_P | mem::pt_flag::FLAG_W;
    pageTable.mapPage(flags, KERNEL_STACK_BOTTOM, flags, false, page_size::pt_2MB, physAddr);
}

static void initSyscallStack(PageTable& pageTable)
{
    // Define Syscall Stack
    std::uint16_t flags = mem::pt_flag::FLAG_P | mem::pt_flag::FLAG_W;
    mem::Page* syscallStackPage = alloc_zeroed_pages(9);
    std::uint64_t syscallStackPagePhysAddr = syscallStackPage->getPhysicalAddr();
    pageTable.mapPage(flags, SYSCALL_STACK_BOTTOM, flags, false, page_size::pt_2MB, syscallStackPagePhysAddr);
}

MemoryDescriptor::MemoryDescriptor(PageTable pageTable) : _pageTable(pageTable) {}

MemoryDescriptor::MemoryDescriptor(const MemoryDescriptor& kernelMemDesc)
{
    // Copy kernel pagetable
    for (auto i = 256; i < 512; ++i) {
        if (i == RESERVED_KERNEL_PAGE_TABLE_LVL4) {
            // Space for the kernel/syscall stack, not copied from parent
            continue;
        }
        _pageTable.copyLevel4Page(kernelMemDesc._pageTable, i);
    }

    initSyscallStack(_pageTable);
}

void MemoryDescriptor::setKernelStack(std::uint64_t physAddr)
{
    initKernelStack(_pageTable, physAddr);
}

void MemoryDescriptor::load(utils::Elf64File& elf64File, fs::File* file)
{
    for (std::size_t i = 0; i < elf64File.getFileHeader().e_phnum; ++i) {
        auto pHdr = elf64File.getProgramHeader(i);
        if (pHdr.p_type != utils::Elf64::ProgramType::PT_LOAD) {
            continue;
        }

        std::uint64_t addr = utils::alignDown(pHdr.p_vaddr, PAGE_SIZE);
        std::size_t length = utils::alignUp(pHdr.p_vaddr + pHdr.p_memsz, PAGE_SIZE) - addr;
        std::uint32_t prot = MemoryRegion::Prot::NONE;
        std::uint32_t flags = 0;

        prot |= MemoryRegion::Prot::READ;
        if (pHdr.p_flags & utils::Elf64::ProgramFlag::PF_W) {
            prot |= MemoryRegion::Prot::WRITE;
        }
        if (pHdr.p_flags & utils::Elf64::ProgramFlag::PF_X) {
            prot |= MemoryRegion::Prot::EXEC;
        }

        if (prot & MemoryRegion::Prot::WRITE) {
            flags |= MemoryRegion::Flags::PRIVATE;
        } else {
            flags |= MemoryRegion::Flags::SHARED;
        }

        off_t offset = utils::alignDown(pHdr.p_offset, PAGE_SIZE);

        mmap(addr, length, prot, flags, file, offset);
    }
}

std::uint64_t MemoryDescriptor::mmap(std::uint64_t addr, std::size_t length, std::uint32_t prot, std::uint32_t flags,
                                     fs::File* file, off_t offset)
{
    addr = utils::alignUp(addr, PAGE_SIZE);

    if (length == 0) {
        return (std::uint64_t)EINVAL;
    }

    if (!std::has_single_bit((flags & (MemoryRegion::Flags::PRIVATE | MemoryRegion::Flags::SHARED)))) {
        return (std::uint64_t)EINVAL;
    }

    if (flags & MemoryRegion::Flags::ANON) {
        offset = 0;
        file = nullptr;
    } else {
        if (offset % PAGE_SIZE != 0) {
            return (std::uint64_t)EINVAL;
        }
    }

    MemoryRegion region{};
    region.begin = addr;
    region.end = addr + offset;
    region.prot = prot;
    region.flags = flags;
    region.file = file;
    region.offset = offset;

    _memoryRegions.push_back(region);

    return addr;
}

}
