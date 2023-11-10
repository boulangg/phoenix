/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Kernel.h"

#include "GlobalDescTable.h"
#include "utils/Elf64File.h"

namespace kernel {

core::InterruptDispatcher Kernel::_interrupt;

mem::MemoryAllocator Kernel::_memory;
mem::PageTable Kernel::_kernelPageTable __attribute__((aligned(4096)));

typedef void (*func_ptr)(void);
extern "C" func_ptr _init_array_start[0], _init_array_end[0];

void Kernel::setupGlobalConstructors()
{
    for (auto fn = _init_array_start; fn != _init_array_end; fn++) {
        (*fn)();
    }
}

void Kernel::start(KernelInfo& info)
{
    utils::Elf64File kernelFile(info.kernelFileAddr);

    // Generic CPU struct
    GDT::setupGDT();
    TSS::setupTSS();

    // Memory related constant
    kernel::mem::Page::KERNEL_BASE_LINEAR_MAPPING = info.hhdm;
    // Memory Allocator

    _memory.init(info.pageArray, info.pageCount);

    // Kernel base Page Table
    mem::PageTable::initKernelPageTable(&_kernelPageTable, info.hhdm, info.pageCount, info.kernelPhysBase, kernelFile,
                                        &_memory);
    set_CR3(_kernelPageTable.getPageTablePhysAddr());

    // Interrupts
    _interrupt.init();

    //_device.init();
    //_scheduler.init();
}

}
