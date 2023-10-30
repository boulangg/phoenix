/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "Constant.h"
#include "mem/Page.h"

#include <cstdarg>

namespace kernel {

mem::Page* alloc_page();
mem::Page* alloc_pages(std::size_t order);
mem::Page* alloc_zeroed_page();
mem::Page* alloc_zeroed_pages(std::size_t order);
void free_page(mem::Page* page);
void free_pages(mem::Page* page, std::size_t order);

void schedule();

void printk(const char* format, ...);
void printk(const char* format, va_list args);

}
