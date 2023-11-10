/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstdint>

namespace kernel::mem {

namespace MemoryRegionFlags {
static constexpr std::uint32_t READ = 0x01;
static constexpr std::uint32_t WRITE = 0x02;
static constexpr std::uint32_t EXEC = 0x03;
};

struct MemoryRegion
{
    std::uint64_t begin;
    std::uint64_t end;
    std::uint32_t flags; /// R/W/E
};

}
