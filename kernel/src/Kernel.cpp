/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Kernel.h"

#include "GlobalDescTable.h"

namespace kernel {

Kernel Kernel::kernel;

Kernel::Kernel(){};

typedef void (*func_ptr)(void);
extern "C" func_ptr _init_array_start[0], _init_array_end[0];

void Kernel::setupGlobalConstructors()
{
    for (auto fn = _init_array_start; fn != _init_array_end; fn++) {
        (*fn)();
    }
}


void Kernel::start(mem::Page* pageArray, std::size_t pageCount)
{
    GDT::setupGDT();
    TSS::setupTSS();
    _memory.init(pageArray, pageCount);
    _interrupt.init();
    setupGlobalConstructors();
    //_device.init();
    //_scheduler.init();
}

}
