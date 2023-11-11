/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>

#include "core/InterruptDispatcher.h"
#include "mem/AddressSpace.h"
#include "mem/MemoryAllocator.h"

namespace kernel {

struct KernelInfo
{
    std::uint64_t pageArray;
    std::size_t pageCount;
    std::size_t hhdm;
    std::size_t kernelFileAddr;
    std::size_t kernelPhysBase;
    std::size_t kernelVirtBase;
};

class Kernel
{
public:
    static void init(KernelInfo& info);

private:
    static mem::MemoryAllocator _memory;
    static mem::AddressSpace* _kernelAddressSpace;
    static core::InterruptDispatcher _interrupt;

    // dev::DeviceExplorer _device;
    // proc::ProcessScheduler _scheduler;
};

}
