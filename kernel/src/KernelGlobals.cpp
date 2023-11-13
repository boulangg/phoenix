/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "KernelGlobals.h"

#include "Kernel.h"

namespace kernel {

#define BUFFER_SIZE 1024
static char printkBuffer[BUFFER_SIZE];

mem::Page* alloc_page()
{
    return Kernel::memory.allocPage();
}

mem::Page* alloc_pages(std::size_t order)
{
    return Kernel::memory.allocPages(order);
}

mem::Page* alloc_zeroed_page()
{
    return Kernel::memory.allocZeroedPage();
}

mem::Page* alloc_zeroed_pages(std::size_t order)
{
    return Kernel::memory.allocZeroedPages(order);
}

void schedule()
{
    Kernel::scheduler->schedule();
}

void printk(const char* format, ...)
{
    va_list vl;
    vsnprintf(printkBuffer, BUFFER_SIZE, format, vl);
    // Kernel::Console::write(tmp);
}

}
