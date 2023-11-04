/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "Kernel.h"

#include "Segment.h"

namespace kernel {

Kernel Kernel::kernel;

Kernel::Kernel(){};

void Kernel::start(mem::Page* pageArray, std::size_t pageCount)
{
    GDT::setupGDT();
    TSS::setupTSS();
    _memory.init(pageArray, pageCount);
    //_interrupt.init();
    //_device.init();
    //_scheduler.init();
}

}
