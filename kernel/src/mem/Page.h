/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <atomic>

#include "Constant.h"

namespace kernel::mem {

struct Page
{
    enum Type
    {
        UNUSABLE,
        FREE,
        ALLOCATED,
        KERNEL,
        CACHE
    };

    std::size_t index;
    Type type;

    // Used by the buddy alogrithm in the MemoryAllocator for free pages
    Page* nextFreeBlock;

    std::uint64_t getKernelAddr()
    {
        return KERNEL_BASE_LINEAR_MAPPING + index * PAGE_SIZE;
    }

    std::uint64_t getPhysicalAddr()
    {
        return index * PAGE_SIZE;
    }

    void lock();
    void unlock();

private:
    std::atomic_flag _locked;
};

}
