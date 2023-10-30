/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at
 * https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "BlockCache.h"

#include "KernelGlobals.h"

#include "BlockDevice.h"

namespace kernel::dev {

BlockCache::BlockCache(BlockDevice* blkDev, std::uint64_t logicalBlockShift) :
    _blkDev(blkDev), _logicalBlockShift(logicalBlockShift)
{
    _blkPerPage = PAGE_SIZE >> logicalBlockShift;
    _logicalToPhysicalBlkShift = logicalBlockShift - _blkDev->getBlockShift();
}

BlockDescriptor BlockCache::getBlock(std::uint64_t logBlkNum)
{
    BlockDescriptor blockDescriptor{.blkDev = _blkDev,
                                    .blkNum = logBlkNum << _logicalToPhysicalBlkShift,
                                    .blkSize = 1ull << _logicalBlockShift,
                                    .page = nullptr,
                                    .offset = (logBlkNum % _blkPerPage) << _logicalToPhysicalBlkShift};

    std::uint64_t pageNum = logBlkNum / _blkPerPage;
    if (!_pageMap.contains(pageNum)) {
        mem::Page* p = alloc_page();
        p->lock();
        _blkDev->readPage(p, pageNum << _blkDev->getBlockShift(), PAGE_SIZE >> _blkDev->getBlockShift());
        _pageMap[pageNum] = p;
        p->unlock();
    }
    blockDescriptor.page = _pageMap.at(pageNum);

    return blockDescriptor;
}

}
