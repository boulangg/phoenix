/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at
 * https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>
#include <unordered_map>

#include "BlockDevice.h"

#include "mem/Page.h"

namespace kernel::dev {

class BlockCache
{
public:
    BlockCache(BlockDevice* blkDev, std::uint64_t logicalBlockSize);

    BlockDescriptor getBlock(std::uint64_t blkNum);

private:
    BlockDevice* _blkDev;
    std::uint64_t _logicalBlockShift;
    std::uint64_t _blkPerPage;
    std::uint64_t _logicalToPhysicalBlkShift;
    std::unordered_map<std::uint64_t, mem::Page*> _pageMap;
};

}
