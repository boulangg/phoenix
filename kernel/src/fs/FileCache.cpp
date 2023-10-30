/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "FileCache.h"

#include "Inode.h"

namespace kernel::fs {

FileCache::FileCache(Inode* inode) : _inode(inode) {}

FileCache::~FileCache() {}

ssize_t FileCache::read(char* buffer, loff_t offset, std::size_t size)
{
    std::size_t current = offset;
    while (current < offset + size) {
        std::size_t pageNum = current / PAGE_SIZE;
        if (!_cache.contains(pageNum)) {
            mem::Page* p = alloc_page();
            _inode->readpage(p, pageNum);
            _cache[pageNum] = p;
        }

        mem::Page* p = _cache[pageNum];

        std::size_t next = std::min<std::size_t>(offset + size, (pageNum + 1) * PAGE_SIZE);
        std::size_t count = next - current;
        memcpy(buffer, (char*)(p->getKernelAddr() + current), count);
        buffer += count;
        current += count;
    }

    return current - offset;
}

}
