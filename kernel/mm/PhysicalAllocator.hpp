/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _MM_PHYSICALALLOCATOR_HPP_
#define _MM_PHYSICALALLOCATOR_HPP_

#define MAX_ORDER 64

#include <cstdint>

#include <include/constant.h>
#include "Page.hpp"

class PhysicalAllocator {
public:
	static void initAllocator(Page* page_array, uint64_t nb_physical_pages);
	static Page* allocPage();
	static Page* allocPages(uint8_t order);

	static Page* allocZeroedPage();
	static Page* allocZeroedPages(uint8_t order);

	static int freePage(Page* page);
	static int freePages(Page* page, uint8_t order);
	static void printStats();

	static Page* getPageFromAddr(uint64_t* addr);

private:
	static uint64_t* allocPagesInternal(uint8_t order);
	static int freePagesInternal(uint64_t* addr, uint8_t order);

	static Page* page_array;
	static uint64_t nb_physical_pages;

	// Free bloc table
	static uint64_t FBT[MAX_ORDER];

	static uint64_t* getAddrFromPage(Page* page);
	static uint64_t* calculAddrBuddy(uint64_t* addr, uint8_t order);
	static uint64_t* fusion(uint64_t* addr, uint8_t order);
};

#endif /* _MM_PHYSICALALLOCATOR_HPP_ */
