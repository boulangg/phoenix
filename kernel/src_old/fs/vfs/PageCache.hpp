#pragma once

#include <cstdint>
#include <list>

#include <mm/Page.hpp>
#include <mm/PhysicalAllocator.hpp>
#include <fs/vfs/AddressSpace.hpp>

class PageCache
{

public:
	static Page* getPage(AddressSpace* mapping, std::uint64_t index)
	{
// Check if page exist
		for (Page* page : pages) {
			if (page->mapping == mapping && page->offset == index) {
				return page;
			}
		}

		Page* p = PhysicalAllocator::allocPage();
		p->mapping = mapping;
		p->offset = index;
		pages.push_back(p);
		return p;
	}

	static std::list<Page*> pages;
};
