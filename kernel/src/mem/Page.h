/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

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

    void setCounter(std::uint64_t value)
    {
        _counter.store(value);
    }

    std::uint64_t decrementCounter()
    {
        return _counter--;
    }

private:
    std::atomic_flag _locked;
    std::atomic_uint64_t _counter;
};

}
