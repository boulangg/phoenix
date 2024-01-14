/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "BlockCache.h"

#include "KernelGlobals.h"

#include "BlockDevice.h"

namespace kernel::dev {

BlockCache::BlockCache(BlockDevice* blkDev) : _blkDev(blkDev)
{
    _blkPerPage = PAGE_SIZE / _blkDev->getBlockSize();
}

BlockDescriptor BlockCache::getBlock(std::uint64_t blkNum)
{
    BlockDescriptor blockDescriptor{
        .blkDev = _blkDev,
        .blkNum = blkNum, 
        .blkSize = _blkDev->getBlockSize(),
        .page = nullptr,
        .offset = (blkNum % _blkPerPage) * _blkDev->getBlockSize()
    };

    std::uint64_t pageNum = blkNum / _blkPerPage;
    if (!_pageMap.contains(pageNum)) {
        mem::Page* p = alloc_page();
        p->lock();
        kernel::dev::BlockIORequest request;
        request.start = pageNum * _blkPerPage;
        request.write = false;
        request.entries.push_back(
            BlockIORequestEntry{.page = p, .len = PAGE_SIZE, .offset = 0, .end = [p]() { p->unlock(); }});
        _blkDev->submitRequest(request);
        p->lock();
        _pageMap[pageNum] = p;
        p->unlock();
    }
    blockDescriptor.page = _pageMap.at(pageNum);

    return blockDescriptor;
}

}
