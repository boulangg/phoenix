/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "KernelGlobals.h"

#include "Kernel.h"

namespace kernel {

#define BUFFER_SIZE 1023
static char printkBuffer[BUFFER_SIZE + 1];

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

void free_page(mem::Page* page)
{
    Kernel::memory.freePage(page);
}

void free_pages(mem::Page* page, std::size_t order)
{
    Kernel::memory.freePages(page, order);
}

void schedule()
{
    Kernel::scheduler.schedule();
}

void printk(const char* format, ...)
{
    va_list vl;
    va_start(vl, format);
    printk(format, vl);
    va_end(vl);
}

void printk(const char* format, va_list args)
{
    vsnprintf(printkBuffer, BUFFER_SIZE, format, args);
    printkBuffer[BUFFER_SIZE] = '\0';
    Kernel::write(printkBuffer);
}

}
