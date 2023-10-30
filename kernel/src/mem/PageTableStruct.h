/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "KernelGlobals.h"

namespace kernel::mem {

struct pt_flag
{
    static constexpr std::uint16_t FLAG_P = (1 << 0);         // Present
    static constexpr std::uint16_t FLAG_W = (1 << 1);         // Read/Write
    static constexpr std::uint16_t FLAG_U = (1 << 2);         // User/Supervisor
    static constexpr std::uint16_t FLAG_PWT = (1 << 3);       // Page-Level Writethrough
    static constexpr std::uint16_t FLAG_PCD = (1 << 4);       // Page-Level Cache Disable
    static constexpr std::uint16_t FLAG_A = (1 << 5);         // Accessed
    static constexpr std::uint16_t FLAG_D = (1 << 6);         // Dirty
    static constexpr std::uint16_t FLAG_PTE_PAT = (1 << 7);   // Page-Attribute Table
    static constexpr std::uint16_t FLAG_PDE_PAT = (1 << 12);  // Page-Attribute Table
    static constexpr std::uint16_t FLAG_PDPE_PAT = (1 << 12); // Page-Attribute Table
    static constexpr std::uint16_t FLAG_G = (1 << 8);         // Global Page
    static constexpr std::uint16_t FLAG_AVL_MASK = 0xE00;     // Available to Software
};

enum page_size
{
    pt_4KB,
    pt_2MB,
    pt_1GB
};

static constexpr std::uint64_t PAGE_ADDR_MASK = 0x0000FFFFFFFFF000;
static constexpr std::uint64_t FLAG_PTE_MASK = 0x0000000000000FFF;
static constexpr std::uint64_t FLAG_TOP_LVL_MASK = 0x0000000000000E3F;
static constexpr std::uint64_t FLAG_2MB_PDE = (1 << 7);
static constexpr std::uint64_t FLAG_2MB_PDE_MASK = 0x0000000000001FFF;
static constexpr std::uint64_t FLAG_1GB_PDPE = (1 << 7);
static constexpr std::uint64_t FLAG_1GB_PDPE_MASK = 0x0000000000001FFF;
static constexpr std::uint64_t FLAG_NX = (1ul << 63);

struct pte_t
{
    std::uint64_t _raw;

    std::uint64_t mapPage(uint16_t pageFlags, std::uint64_t physAddr, bool noExec)
    {
        _raw = (physAddr & PAGE_ADDR_MASK) | (pageFlags & FLAG_PTE_MASK);
        if (noExec) {
            _raw |= FLAG_NX;
        }
        return getPhysicalPtr();
    }

    std::uint64_t getPhysicalPtr() const
    {
        return _raw & PAGE_ADDR_MASK;
    }
};

using pt_t = pte_t*;

struct pde_t
{
    std::uint64_t _raw;

    pt_t getPt() const
    {
        std::uint64_t ptr = getPhysicalPtr() | KERNEL_BASE_LINEAR_MAPPING;
        return reinterpret_cast<pt_t>(ptr);
    }

    std::uint64_t mapPage(uint16_t highLvlFlags, uint64_t virtAddr, uint16_t pageFlags, bool noExec, page_size pageSize,
                          std::uint64_t physAddr)
    {
        if (pageSize == page_size::pt_2MB) {
            _raw = (physAddr & PAGE_ADDR_MASK) | (pageFlags & FLAG_2MB_PDE_MASK);
            _raw |= FLAG_2MB_PDE;
            if (noExec) {
                _raw |= FLAG_NX;
            }
            return getPhysicalPtr();
        }

        if (_raw == 0) {
            Page* page = alloc_zeroed_page();
            auto pagePhysAddr = page->getPhysicalAddr();
            _raw = (pagePhysAddr & PAGE_ADDR_MASK) | (highLvlFlags & FLAG_TOP_LVL_MASK);
        }
        _raw |= (highLvlFlags & FLAG_TOP_LVL_MASK);

        uint64_t PDT_index = getIndex(virtAddr);
        pt_t pdt = getPt();
        return pdt[PDT_index].mapPage(pageFlags, physAddr, noExec);
    }

    std::uint64_t getPhysicalPtr() const
    {
        return _raw & PAGE_ADDR_MASK;
    }

private:
    static constexpr std::uint64_t INDEX_OFFSET = 12;
    static constexpr std::uint64_t INDEX_MASK = 0x1FF;

    static std::size_t getIndex(std::uint64_t addr)
    {
        return (addr >> INDEX_OFFSET) & INDEX_MASK;
    }
};

using pdt_t = pde_t*;

struct pdpe_t
{
    std::uint64_t _raw;

    pdt_t getPdt() const
    {
        std::uint64_t ptr = getPhysicalPtr() | KERNEL_BASE_LINEAR_MAPPING;
        return reinterpret_cast<pdt_t>(ptr);
    }

    std::uint64_t mapPage(uint16_t highLvlFlags, uint64_t virtAddr, uint16_t pageFlags, bool noExec, page_size pageSize,
                          std::uint64_t physAddr)
    {
        if (pageSize == page_size::pt_1GB) {
            _raw = (physAddr & PAGE_ADDR_MASK) | (pageFlags & FLAG_1GB_PDPE_MASK);
            _raw |= FLAG_1GB_PDPE;
            if (noExec) {
                _raw |= FLAG_NX;
            }
            return getPhysicalPtr();
        }

        if (_raw == 0) {
            Page* page = alloc_zeroed_page();
            auto pagePhysAddr = page->getPhysicalAddr();
            _raw = (pagePhysAddr & PAGE_ADDR_MASK) | (highLvlFlags & FLAG_TOP_LVL_MASK);
        }
        _raw |= (highLvlFlags & FLAG_TOP_LVL_MASK);

        std::uint64_t PDT_index = getIndex(virtAddr);
        pdt_t pdt = getPdt();
        return pdt[PDT_index].mapPage(highLvlFlags, virtAddr, pageFlags, noExec, pageSize, physAddr);
    }

    std::uint64_t getPhysicalPtr() const
    {
        return _raw & PAGE_ADDR_MASK;
    }

private:
    static constexpr std::uint64_t INDEX_OFFSET = 21;
    static constexpr std::uint64_t INDEX_MASK = 0x1FF;

    static std::size_t getIndex(std::uint64_t addr)
    {
        return (addr >> INDEX_OFFSET) & INDEX_MASK;
    }
};

using pdpt_t = pdpe_t*;

struct pml4e_t
{
    std::uint64_t _raw;

    pdpt_t getPdpt() const
    {
        std::uint64_t ptr = getPhysicalPtr() | KERNEL_BASE_LINEAR_MAPPING;
        return reinterpret_cast<pdpt_t>(ptr);
    }

    std::uint64_t mapPage(uint16_t highLvlFlags, uint64_t virtAddr, uint16_t pageFlags, bool noExec, page_size pageSize,
                          std::uint64_t physAddr)
    {
        if (_raw == 0) {
            Page* page = alloc_zeroed_page();
            auto pagePhysAddr = page->getPhysicalAddr();
            _raw = (pagePhysAddr & PAGE_ADDR_MASK) | (highLvlFlags & FLAG_TOP_LVL_MASK);
        }
        _raw |= (highLvlFlags & FLAG_TOP_LVL_MASK);

        uint64_t PDPT_index = getIndex(virtAddr);
        pdpt_t pdpt = getPdpt();
        return pdpt[PDPT_index].mapPage(highLvlFlags, virtAddr, pageFlags, noExec, pageSize, physAddr);
    }

    std::uint64_t getPhysicalPtr() const
    {
        return _raw & PAGE_ADDR_MASK;
    }

private:
    static constexpr std::uint64_t INDEX_OFFSET = 30;
    static constexpr std::uint64_t INDEX_MASK = 0x1FF;

    static std::size_t getIndex(std::uint64_t addr)
    {
        return (addr >> INDEX_OFFSET) & INDEX_MASK;
    }
};

using pml4t_t = pml4e_t*;

struct page_table
{
    page_table()
    {
        Page* page = alloc_zeroed_page();
        auto pagePhysAddr = page->getPhysicalAddr();
        _raw = pagePhysAddr;
    }

    std::uint64_t _raw;

    pml4t_t getPml4t() const
    {
        std::uint64_t ptr = getPageTablePhysAddr() | KERNEL_BASE_LINEAR_MAPPING;
        return reinterpret_cast<pml4t_t>(ptr);
    }

    std::uint64_t getPageTablePhysAddr() const
    {
        return _raw;
    }

    std::uint64_t mapPage(uint16_t highLvlFlags, uint64_t virtAddr, uint16_t pageFlags, bool noExec, page_size pageSize,
                          std::uint64_t physAddr)
    {
        uint64_t PML4T_index = getIndex(virtAddr);
        pml4t_t pml4t = getPml4t();
        return pml4t[PML4T_index].mapPage(highLvlFlags, virtAddr, pageFlags, noExec, pageSize, physAddr);
    }

    void initLevel4Page(std::size_t index, std::uint64_t value)
    {
        pml4t_t pml4t = getPml4t();
        pml4t[index]._raw = value | pt_flag::FLAG_P;
    }

    void copyLevel4Page(const page_table& table, std::size_t index)
    {
        initLevel4Page(index, table.getPml4t()[index]._raw);
    }

private:
    static std::size_t getIndex(std::uint64_t addr)
    {
        return (addr >> INDEX_OFFSET) & INDEX_MASK;
    }

    static constexpr std::uint64_t INDEX_OFFSET = 39;
    static constexpr std::uint64_t INDEX_MASK = 0x1FF;
};

}
