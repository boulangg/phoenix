/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>

#include "console/BasicConsole.h"
#include "core/InterruptDispatcher.h"
#include "core/rtc/RTCDevice.h"
#include "mem/MemoryAllocator.h"
#include "mem/MemoryDescriptor.h"
#include "proc/ProcessScheduler.h"

namespace kernel {

struct KernelInfo
{
    std::uint64_t pageArray;
    std::size_t pageCount;
    std::size_t hhdm;
    std::size_t kernelFileAddr;
    std::size_t kernelFileSize;
    std::size_t kernelPhysBase;
    std::size_t kernelVirtBase;
    console::Framebuffer framebuffer;
};

class Kernel
{
public:
    static void init(const KernelInfo& info);
    static void start();
    static void write(const char* str);
    static core::rtc::DateTime getDateTime();

    static mem::MemoryAllocator memory;
    static proc::ProcessScheduler scheduler;
    static core::InterruptDispatcher interrupt;

private:
    static mem::MemoryDescriptor* _kernelMemDesc;
    static console::BasicConsole _console;
    static core::rtc::RTCDevice* _rtc;

    // dev::DeviceExplorer _device;
};

}
