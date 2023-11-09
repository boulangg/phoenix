/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstddef>
#include <cstdint>
#include <list>

#include "MemoryAllocator.h"
#include "Page.h"

namespace kernel::mem {

struct pt_flag
{
    static constexpr std::uint16_t FLAG_P = (1 << 0);     // Present
    static constexpr std::uint16_t FLAG_W = (1 << 1);     // Read/Write
    static constexpr std::uint16_t FLAG_U = (1 << 2);     // User/Supervisor
    static constexpr std::uint16_t FLAG_PWT = (1 << 3);   // Page-Level Writethrough
    static constexpr std::uint16_t FLAG_PCD = (1 << 4);   // Page-Level Cache Disable
    static constexpr std::uint16_t FLAG_A = (1 << 5);     // Accessed
    static constexpr std::uint16_t FLAG_D = (1 << 6);     // Dirty
    static constexpr std::uint16_t FLAG_PAT = (1 << 7);   // Page-Attribute Table (for PTE)
    static constexpr std::uint16_t FLAG_G = (1 << 8);     // Global Page
    static constexpr std::uint16_t FLAG_AVL_MASK = 0xE00; // Available to Software
};

enum page_size
{
    pt_2KiB, 
    pt_2MiB
};

static constexpr std::uint64_t PAGE_ADDR_MASK = ~(PAGE_SIZE - 1);

struct pte_t
{
    union
    {
        struct
        {
            std::uint8_t nx         : 1; // No-execute page-protection
            std::uint8_t mpk        : 4; // Memory Protection Key (if enabled)
            std::uint16_t available : 7;
            std::uint64_t page_base : 40;
            std::uint16_t flags     : 12;
        } __attribute__((packed));
        std::uint64_t _raw;
    } __attribute__((packed));

    std::list<Page*> mapPage(MemoryAllocator allocator, uint16_t pageFlags, uint64_t physAddr, bool noExec)
    {
        Page* page = nullptr;
        if (physAddr == 0) {
            page = allocator.allocZeroedPage();
            physAddr = page->getPhysicalAddr();
        }
        _raw = physAddr | pageFlags;
        nx = noExec;

        std::list<Page*> res;
        if (page != nullptr) {
            res.push_back(page);
        }
        return res;
    }

    std::uint64_t getPhysicalPtr() const
    {
        return _raw & PAGE_ADDR_MASK;
    }

} __attribute__((packed));
static_assert(sizeof(pte_t) == 8, "pte size incorrect");
using pt_t = pte_t*;

struct pde_t
{
    union
    {
        struct
        {
            std::uint8_t nx         : 1; // No-execute page-protection
            std::uint16_t available : 11;
            std::uint64_t pt_base   : 40;
            std::uint16_t flags     : 12;
        } __attribute__((packed));
        std::uint64_t _raw;
    } __attribute__((packed));

    pt_t getPt() const
    {
        std::uint64_t ptr = getPhysicalPtr() | kernel::mem::Page::KERNEL_BASE_LINEAR_MAPPING;
        return reinterpret_cast<pt_t>(ptr);
    }

    std::list<Page*> mapPage(MemoryAllocator allocator, uint16_t highLvlFlags, uint64_t virtAddr, uint16_t pageFlags,
                             bool noExec, uint64_t physAddr)
    {
        Page* page = nullptr;
        if (_raw == 0) {
            page = allocator.allocZeroedPage();
            _raw = page->getPhysicalAddr() | highLvlFlags;
        }
        uint64_t PDT_index = getIndex(virtAddr);
        pt_t pdt = getPt();
        auto res = pdt[PDT_index].mapPage(allocator, pageFlags, physAddr, noExec);
        if (page != nullptr) {
            res.push_back(page);
        }
        return res;
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
} __attribute__((packed));
static_assert(sizeof(pde_t) == 8, "pde size incorrect");
using pdt_t = pde_t*;

struct pdpe_t
{
    union
    {
        struct
        {
            std::uint8_t nx         : 1; // No-execute page-protection
            std::uint16_t available : 11;
            std::uint64_t pdt_base  : 40;
            std::uint16_t flags     : 12;
        } __attribute__((packed));
        std::uint64_t _raw;
    } __attribute__((packed));

    pdt_t getPdt() const
    {
        std::uint64_t ptr = getPhysicalPtr() | kernel::mem::Page::KERNEL_BASE_LINEAR_MAPPING;
        return reinterpret_cast<pdt_t>(ptr);
    }

    pt_t getPt() const
    {
        std::uint64_t ptr = getPhysicalPtr() | kernel::mem::Page::KERNEL_BASE_LINEAR_MAPPING;
        return reinterpret_cast<pt_t>(ptr);
    }

    std::list<Page*> mapPage(MemoryAllocator allocator, uint16_t highLvlFlags, uint64_t virtAddr, uint16_t pageFlags,
                             bool noExec, page_size pageSize, uint64_t physAddr)
    {
        Page* page = nullptr;
        if (_raw == 0) {
            page = allocator.allocZeroedPage();
            _raw = page->getPhysicalAddr() | highLvlFlags;
        }

        std::list<Page*> res; 
        std::uint64_t PDT_index = getIndex(virtAddr);
        if (pageSize == page_size::pt_2MiB) {
            pt_t pt = getPt();
            res = pt[PDT_index].mapPage(allocator, pageFlags, physAddr, noExec);
        } else {
            pdt_t pdt = getPdt();
            res = pdt[PDT_index].mapPage(allocator, highLvlFlags, virtAddr, pageFlags, noExec, physAddr);
        }

        if (page != nullptr) {
            res.push_back(page);
        }
        return res;
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
} __attribute__((packed));
static_assert(sizeof(pdpe_t) == 8, "pdpe size incorrect");
using pdpt_t = pdpe_t*;

struct pml4e_t
{
    union
    {
        struct
        {
            std::uint8_t nx         : 1; // No-execute page-protection
            std::uint16_t available : 11;
            std::uint64_t pdpt_base : 40;
            std::uint16_t flags     : 12;
        } __attribute__((packed));
        std::uint64_t _raw;
    } __attribute__((packed));

    pdpt_t getPdpt() const
    {
        std::uint64_t ptr = getPhysicalPtr() | kernel::mem::Page::KERNEL_BASE_LINEAR_MAPPING;
        return reinterpret_cast<pdpt_t>(ptr);
    }

    std::list<Page*> mapPage(MemoryAllocator allocator, uint16_t highLvlFlags, uint64_t virtAddr, uint16_t pageFlags,
                             bool noExec, page_size pageSize, uint64_t physAddr)
    {
        Page* page = nullptr;
        if (_raw == 0) {
            page = allocator.allocZeroedPage();
            _raw = page->getPhysicalAddr() | highLvlFlags;
        }
        uint64_t PDPT_index = getIndex(virtAddr);
        pdpt_t pdpt = getPdpt();
        auto res = pdpt[PDPT_index].mapPage(allocator, highLvlFlags, virtAddr, pageFlags, noExec, pageSize, physAddr);
        if (page != nullptr) {
            res.push_back(page);
        }
        return res;
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
} __attribute__((packed));
static_assert(sizeof(pml4e_t) == 8, "pml4e size incorrect");
using pml4t_t = pml4e_t*;

struct page_table
{
    union
    {
        pml4e_t _pml4t[512];
    };

    pml4t_t getPml4t() const
    {
        std::uint64_t ptr = reinterpret_cast<std::uint64_t>(this);
        return reinterpret_cast<pml4t_t>(ptr);
    }

    std::list<Page*> mapPage(MemoryAllocator allocator, uint16_t highLvlFlags, uint64_t virtAddr, uint16_t pageFlags,
                             bool noExec, page_size pageSize, uint64_t physAddr)
    {
        uint64_t PML4T_index = getIndex(virtAddr);
        pml4t_t pml4t = getPml4t();
        return pml4t[PML4T_index].mapPage(allocator, highLvlFlags, virtAddr, pageFlags, noExec, pageSize, physAddr);
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
