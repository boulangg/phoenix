/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "MemoryInit.h"

#include <algorithm>
#include <cstdint>

#define BASE_PAGE_ARRAY_MAPPING (2ul << 24) // 16 Mo

namespace kernel::mem {

static std::pair<std::uint64_t, std::uint64_t> base_alloc_pages(std::uint64_t nb_pages, limine_memmap_request memmap_request)
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
            return {baseStart, baseStart + nb_pages * PAGE_SIZE};
        }
    }

    return {0ul, 0ul};
}

static void setPageType(Page* pageArray, std::uint64_t indexStart, std::uint64_t indexEnd, Page::Type type)
{
    for (auto index = indexStart / PAGE_SIZE; index < indexEnd; ++index) {
        pageArray[index].type = type;
    }
}

std::pair<Page*, std::size_t> initPageArray(const limine_memmap_request& memmap_request)
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
    Page* pageArray = reinterpret_cast<Page*>(allocPages.first);

    // Initialize the pageArray
    for (std::uint64_t i = 0; i < nb_physical_pages; ++i) {
        pageArray[i].index = i;
        //pageArray[i].physAddr = (uint64_t*)(i * PAGE_SIZE);
        //pageArray[i].kernelMappAddr = (uint64_t*)(i * PAGE_SIZE + hhdm_request.response->offset);
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
            setPageType(pageArray, indexStart, indexEnd, Page::Type::FREE);
            break;
        case LIMINE_MEMMAP_RESERVED:
            setPageType(pageArray, indexStart, indexEnd, Page::Type::UNUSABLE);
            break;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            setPageType(pageArray, indexStart, indexEnd, Page::Type::UNUSABLE);
            break;
        case LIMINE_MEMMAP_ACPI_NVS:
            setPageType(pageArray, indexStart, indexEnd, Page::Type::UNUSABLE);
            break;
        case LIMINE_MEMMAP_BAD_MEMORY:
            setPageType(pageArray, indexStart, indexEnd, Page::Type::UNUSABLE);
            break;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            setPageType(pageArray, indexStart, indexEnd, Page::Type::ALLOCATED);
            break;
        case LIMINE_MEMMAP_KERNEL_AND_MODULES:
            setPageType(pageArray, indexStart, indexEnd, Page::Type::KERNEL);
            break;
        case LIMINE_MEMMAP_FRAMEBUFFER:
            setPageType(pageArray, indexStart, indexEnd, Page::Type::UNUSABLE);
            break;
        }
    }

    // Allocate memory used for the page_array
    auto indexStart = allocPages.first / PAGE_SIZE;
    auto indexEnd = allocPages.second / PAGE_SIZE;
    setPageType(pageArray, indexStart, indexEnd, Page::Type::KERNEL);

    return {pageArray, nb_physical_pages};
}

}
