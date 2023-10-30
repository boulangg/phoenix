/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <mm/PhysicalAllocator.hpp>
#include <mm/PhysicalMapping.hpp>

PhysicalMapping::PhysicalMapping(uint64_t nb)
{
    Page* nullPage = nullptr;
    for (uint64_t i = 0; i < nb; ++i) {
        array.push_back(nullPage);
    }
}

PhysicalMapping::PhysicalMapping(const PhysicalMapping& mapping)
{
    for (uint64_t i = 0; i < mapping.size(); ++i) {
        if (mapping.array[i] != nullptr) {
            Page* newPage = PhysicalAllocator::allocPage();
            Page::copyPage(mapping.array[i], newPage);
            array.push_back(newPage);
        } else {
            array.push_back(nullptr);
        }
    }
}

PhysicalMapping::~PhysicalMapping() {}

void PhysicalMapping::write(const char* src, int offset, int size)
{
    int start = offset;
    int end = offset + size;
    while (start < end) {
        int numPage = start / PAGE_SIZE;
        int startPage = start % PAGE_SIZE;
        int endPage = PAGE_SIZE;
        if (numPage == end / PAGE_SIZE) {
            endPage = end % PAGE_SIZE;
        }
        int size = endPage - startPage;
        Page* pg = getPage(numPage);
        if (pg == nullptr) {
            pg = PhysicalAllocator::allocZeroedPage();
            setPage(numPage, pg);
        }
        writeOnPage(pg, src + start - offset, startPage, size);
        start += size;
    }
}

void PhysicalMapping::read(const char* src, int offset, int size)
{
    (void)src;
    (void)offset;
    (void)size;
}

uint64_t PhysicalMapping::size() const
{
    return array.size();
}

void PhysicalMapping::setPage(uint64_t index, Page* physAddr)
{
    array[index] = physAddr;
}

Page* PhysicalMapping::getPage(uint64_t index)
{
    return array[index];
}

PhysicalMapping* PhysicalMapping::pushBack(PhysicalMapping* map)
{
    for (uint64_t i = 0; i < map->size(); ++i) {
        array.push_back(array[i]);
    }
    return this;
}
