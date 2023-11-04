/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstddef>

#include "core/InterruptDispacther.h"
#include "dev/DeviceExplorer.h"
#include "mem/MemoryAllocator.h"
#include "proc/ProcessScheduler.h"

namespace kernel {
class Kernel
{
public:
    Kernel();

    void start(mem::Page* pageArray, std::size_t pageCount);

    static Kernel kernel;

private:
    mem::MemoryAllocator _memory;
    //core::InterruptDispacther _interrupt;
    //dev::DeviceExplorer _device;
    //proc::ProcessScheduler _scheduler;

};

}
