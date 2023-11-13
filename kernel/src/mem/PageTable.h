/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>

#include "Page.h"
#include "PageTableStruct.h"

#include "utils/Elf64File.h"

namespace kernel::mem {

using PageTable = page_table;

}

// ASM functions
#ifdef __cplusplus
extern "C"
{
#endif

extern void set_CR3(std::uint64_t pml4t);
extern std::uint64_t get_CR3();

#ifdef __cplusplus
}
#endif
