/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>

#include "Page.h"

namespace kernel::mem {

class MemoryAllocator
{
    static constexpr std::int8_t MAX_ORDER = 48;

public:
    MemoryAllocator();
    void init(std::uint64_t pageArray, std::size_t pageCount);

    // Page allocation
    Page* allocPage();
    Page* allocPages(std::uint8_t order);

    Page* allocZeroedPage();
    Page* allocZeroedPages(std::uint8_t order);

    void freePage(Page* page);
    void freePages(Page* page, std::uint8_t order);

private:
    Page* _pageArray;
    std::size_t _pageCount;
    Page* _FBT[MAX_ORDER];

    std::size_t fusion(std::size_t index, std::uint8_t order);
    std::size_t getBuddyIndex(std::size_t index, std::uint8_t order);
    std::size_t allocPageInternal(std::uint8_t order);
    void freePagesInternal(std::size_t index, std::uint8_t order);
};

}
