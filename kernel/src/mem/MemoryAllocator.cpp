/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "MemoryAllocator.h"

#include <algorithm>
#include <bit>

namespace kernel::mem {

static int test_free_block(Page* page_array, std::size_t start, std::size_t end)
{
    for (std::size_t i = start; i < end; i++) {
        if (page_array[i].type != Page::Type::FREE) {
            return 0;
        }
    }
    return 1;
}

// Never initialize members of MemoryAllocator. The constructor is called in SetupGlobalConstructors but we have already called init
// to initialize it in the kernel startup sequence before
MemoryAllocator::MemoryAllocator() {}

void MemoryAllocator::init(std::uint64_t pageArray, std::size_t pageCount)
{
    _pageArray = reinterpret_cast<Page*>(pageArray);
    _pageCount = pageCount;

    for (std::int8_t i = 0; i < MAX_ORDER; i++) {
        _FBT[i] = 0;
    }

    std::size_t i = 1; // The page 0 is never available
    while (i < _pageCount) {
        std::size_t tmp = i;
        std::int8_t max_order = std::countr_zero(tmp);
        std::int8_t order = -1;
        std::size_t start = i;
        std::size_t end = i + 1;
        while (order < max_order && test_free_block(_pageArray, start, end)) {
            order++;
            start = end;
            end += (1ul << order);
        }

        if (order >= 0) {
            freePagesInternal(i, order);
            i += (1ul << order);
        } else {
            i++;
        }
    }
}

Page* MemoryAllocator::allocPage()
{
    return allocPages(0);
}

Page* MemoryAllocator::allocPages(std::uint8_t order)
{
    std::size_t index = allocPageInternal(order);
    if (index == 0) {
        return nullptr;
    }
    return _pageArray + index;
}

Page* MemoryAllocator::allocZeroedPage()
{
    return allocZeroedPages(0);
}

Page* MemoryAllocator::allocZeroedPages(std::uint8_t order)
{
    Page* page = allocPages(order);

    if (page == nullptr) {
        return nullptr;
    }

    std::size_t size = (1 << order) * PAGE_SIZE / sizeof(std::uint64_t);
    std::uint64_t* startAddr = (uint64_t*)page->getKernelAddr();
    std::fill_n(startAddr, size, 0);

    return page;
}

void MemoryAllocator::freePage(Page* page)
{
    return freePages(page, 0);
}

void MemoryAllocator::freePages(Page* page, std::uint8_t order)
{
    freePagesInternal(page->index, order);
}

std::size_t MemoryAllocator::fusion(std::size_t index, std::uint8_t order)
{
    std::size_t buddyIndex = getBuddyIndex(index, order);
    Page** curr = _FBT + order;

    /* Look for buddy bloc inside the free bloc list for the
     * current order */
    while ((*curr) != nullptr && (*curr)->index < buddyIndex) {
        // bloc already free
        if ((*curr)->index == index) {
            return 0;
        }
        curr = &((*curr)->nextFreeBlock);
    }

    /* If the buddy is found, we remove the buddy from the free list
     * and return the lowest address of the two blocs that now form one */
    if ((*curr) != nullptr && (*curr)->index == buddyIndex) {
        *curr = (*curr)->nextFreeBlock;
        return std::min(index, buddyIndex);
    }

    /* If we reach the end of the list, we insert the new bloc inside the list */
    _pageArray[index].nextFreeBlock = (*curr);
    (*curr) = _pageArray + index;
    return 0;
}

std::size_t MemoryAllocator::getBuddyIndex(std::size_t index, std::uint8_t order)
{
    return index ^ (1ul << order);
}

std::size_t MemoryAllocator::allocPageInternal(std::uint8_t order)
{
    uint8_t k0 = order;
    /** We look for a bloc big enough to provide the demanded size */
    while (_FBT[k0] == 0) {
        k0++;
        /* If we reach the end of the FBT, we can't satisfy the allocation */
        if (k0 > MAX_ORDER) {
            return 0;
        }
    }

    /* Remove first bloc from FBT */
    std::size_t index = _FBT[k0]->index;
    _FBT[k0] = _FBT[k0]->nextFreeBlock;

    /* Fragment bloc until reaching desired size and save buddies in table */
    while (k0 > order) {
        k0--;
        std::size_t buddyIndex = getBuddyIndex(index, k0);
        _FBT[k0] = _pageArray + buddyIndex;
        _FBT[k0]->nextFreeBlock = nullptr;
    }

    for (uint64_t i = 0; i < (1ul << order); ++i) {
        _pageArray[index + i].type = Page::Type::ALLOCATED;
        _pageArray[index + i].nextFreeBlock = nullptr;
    }

    return index;
}

void MemoryAllocator::freePagesInternal(std::size_t index, std::uint8_t order)
{
    for (std::size_t i = 0; i < (1ul << order); ++i) {
        _pageArray[i].type = Page::Type::FREE;
        _pageArray[i].nextFreeBlock = nullptr;
    }

    std::size_t tmp = index;
    /* Fusion of blocs when possible */
    while (tmp != 0) {
        tmp = fusion(tmp, order);
        order++;
    }
}

}
