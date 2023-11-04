/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "ProcessorInit.h"

gdt_desc gdt[GDT_ENTRIES] __attribute__((aligned(4096)));
idt_desc idt[IDT_ENTRIES] __attribute__((aligned(4096)));

namespace kernel::processor {

static void fill_segment_descriptor(std::uint8_t index, std::uint64_t base, std::uint32_t limit, std::uint8_t flags,
                                    std::uint8_t access)
{
    gdt[index].access = access;
    gdt[index].flags = flags;
    gdt[index].limit_0 = limit && 0xffff;
    gdt[index].limit_1 = (limit >> 0x10) && 0x0f;
    gdt[index].base_0 = base && 0xffff;
    gdt[index].base_1 = (base >> 0x10) && 0x0f;
    gdt[index].base_2 = (base >> 0x18) && 0x0f;
}

static void setupGDT()
{
    fill_segment_descriptor(GDT::KERNEL_NULL_INDEX, 0x0, 0x0, 0x0, 0x0);
    fill_segment_descriptor(GDT::KERNEL_CS_16_INDEX, GDT::KERNEL_CS_16_BASE, GDT::KERNEL_CS_16_LIMIT,
                            GDT::KERNEL_CS_16_FLAGS, GDT::KERNEL_CS_16_ACCESS);
    fill_segment_descriptor(GDT::KERNEL_DS_16_INDEX, GDT::KERNEL_DS_16_BASE, GDT::KERNEL_DS_16_LIMIT,
                            GDT::KERNEL_DS_16_FLAGS, GDT::KERNEL_DS_16_ACCESS);
    fill_segment_descriptor(GDT::KERNEL_CS_32_INDEX, GDT::KERNEL_CS_32_BASE, GDT::KERNEL_CS_32_LIMIT,
                            GDT::KERNEL_CS_32_FLAGS, GDT::KERNEL_CS_32_ACCESS);
    fill_segment_descriptor(GDT::KERNEL_DS_32_INDEX, GDT::KERNEL_DS_32_BASE, GDT::KERNEL_DS_32_LIMIT,
                            GDT::KERNEL_DS_32_FLAGS, GDT::KERNEL_DS_32_ACCESS);
    fill_segment_descriptor(GDT::KERNEL_CS_64_INDEX, GDT::KERNEL_CS_64_BASE, GDT::KERNEL_CS_64_LIMIT,
                            GDT::KERNEL_CS_64_FLAGS, GDT::KERNEL_CS_64_ACCESS);
    fill_segment_descriptor(GDT::KERNEL_DS_64_INDEX, GDT::KERNEL_DS_64_BASE, GDT::KERNEL_DS_64_LIMIT,
                            GDT::KERNEL_DS_64_FLAGS, GDT::KERNEL_DS_64_ACCESS);
    fill_segment_descriptor(GDT::USER_CS_64_INDEX, GDT::USER_CS_64_BASE, GDT::USER_CS_64_LIMIT, GDT::USER_CS_64_FLAGS,
                            GDT::USER_CS_64_ACCESS);
    fill_segment_descriptor(GDT::USER_DS_64_INDEX, GDT::USER_DS_64_BASE, GDT::USER_DS_64_LIMIT, GDT::USER_DS_64_FLAGS,
                            GDT::USER_DS_64_ACCESS);

    set_GDT(sizeof(gdt_desc) * GDT_ENTRIES - 1, gdt);
}

static void setupIDT() {}

void init()
{
    setupGDT();
    setupIDT();
}

}

#include <algorithm>

#include "limine.h"

#include "../../src/mm/Page.hpp"



std::pair<std::uint64_t, std::uint64_t> base_alloc_pages(std::uint64_t nb_pages, limine_memmap_request memmap_request)
{
    for (std::uint64_t index = 0; index < memmap_request.response->entry_count; ++index) {
        auto& entry = memmap_request.response->entries[index];
        if (entry->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }
        if (entry->base + entry->length < BASE_PAGE_ARRAY_MAPPING) {
            continue;
        }
        std::uint64_t baseStart = std::max(BASE_PAGE_ARRAY_MAPPING, entry->base);
        if (entry->base + entry->length > baseStart + nb_pages * PAGE_SIZE) {
            continue;
        }

        return {baseStart, baseStart + nb_pages * PAGE_SIZE};
    }

    return {0ul, 0ul};
}

void setPageType(std::uint64_t indexStart, std::uint64_t indexEnd, Page::Type type, Page* pageArray)
{
    for (auto index = indexStart / PAGE_SIZE; index < indexEnd; ++index) {
        pageArray[index].type = type;
    }
}

void setupPageAllocator(limine_memmap_request memmap_request, limine_hhdm_request hhdm_request)
{
    // Find highest available memory
    std::uint64_t maxAvailableMemory = 0;
    for (std::uint64_t index = 0; index < memmap_request.response->entry_count; ++index) {
        auto& entry = memmap_request.response->entries[index];
        if (entry->type == LIMINE_MEMMAP_USABLE || entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) {
            maxAvailableMemory = std::max(maxAvailableMemory, entry->base + entry->length);
        }
    }

    // Find enough consecutive available pages to store the pageArray
    std::uint64_t nb_physical_pages = (maxAvailableMemory + PAGE_SIZE - 1) / PAGE_SIZE;
    std::uint64_t nb_required_pages = (nb_physical_pages * sizeof(Page) + PAGE_SIZE - 1) / PAGE_SIZE;
    auto allocPages = base_alloc_pages(nb_required_pages, memmap_request);
    Page* pageArray = (Page*)(allocPages.first);

    // Initialize the pageArray
    for (uint64_t i = 0; i < nb_physical_pages; ++i) {
        pageArray[i].physAddr = (uint64_t*)(i * PAGE_SIZE);
        pageArray[i].kernelMappAddr = (uint64_t*)(i * PAGE_SIZE + hhdm_request.response->offset);
        pageArray[i].type = Page::Type::UNUSABLE;
    }

    // Use memmap to update the pageArray
    for (std::uint64_t index = 0; index < memmap_request.response->entry_count; ++index) {
        auto& entry = memmap_request.response->entries[index];
        if (entry->base > maxAvailableMemory) {
            break;
        }

        auto indexStart = entry->base / PAGE_SIZE;
        auto indexEnd = (entry->base + entry->length) / PAGE_SIZE;

        switch (entry->type) {
        case LIMINE_MEMMAP_USABLE:
            setPageType(indexStart, indexEnd, Page::Type::FREE, pageArray);
            break;
        case LIMINE_MEMMAP_RESERVED:
            setPageType(indexStart, indexEnd, Page::Type::UNUSABLE, pageArray);
            break;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            setPageType(indexStart, indexEnd, Page::Type::UNUSABLE, pageArray);
            break;
        case LIMINE_MEMMAP_ACPI_NVS:
            setPageType(indexStart, indexEnd, Page::Type::UNUSABLE, pageArray);
            break;
        case LIMINE_MEMMAP_BAD_MEMORY:
            setPageType(indexStart, indexEnd, Page::Type::UNUSABLE, pageArray);
            break;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            setPageType(indexStart, indexEnd, Page::Type::ALLOCATED, pageArray);
            break;
        case LIMINE_MEMMAP_KERNEL_AND_MODULES:
            setPageType(indexStart, indexEnd, Page::Type::KERNEL, pageArray);
            break;
        case LIMINE_MEMMAP_FRAMEBUFFER:
            setPageType(indexStart, indexEnd, Page::Type::UNUSABLE, pageArray);
            break;
        }
    }

    // Allocate memory used for the page_array
    auto indexStart = allocPages.first / PAGE_SIZE;
    auto indexEnd = allocPages.second / PAGE_SIZE;
    setPageType(indexStart, indexEnd, Page::Type::KERNEL, pageArray);

    kernel::initPageAllocator(pageArray);
}
