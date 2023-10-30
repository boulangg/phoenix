/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>
#include <sys/mman.h>

#include "sys/types.h"
#include "fs/File.h"

namespace kernel::mem {

struct MemoryRegion
{
    struct Prot
    {
        static constexpr std::uint32_t NONE = PROT_NONE;
        static constexpr std::uint32_t READ = PROT_READ;
        static constexpr std::uint32_t WRITE = PROT_WRITE;
        static constexpr std::uint32_t EXEC = PROT_EXEC;
    };

    struct Flags
    {
        static constexpr std::uint32_t SHARED = MAP_SHARED;
        static constexpr std::uint32_t PRIVATE = MAP_PRIVATE;
        static constexpr std::uint32_t FIXED = MAP_FIXED;
        static constexpr std::uint32_t ANON = MAP_ANON;
    };

    std::uint64_t begin;
    std::uint64_t end;
    std::uint32_t prot;
    std::uint32_t flags;
    fs::File* file;
    off_t offset;
};

}
