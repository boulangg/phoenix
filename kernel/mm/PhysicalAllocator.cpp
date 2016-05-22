/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "PhysicalAllocator.hpp"

#include <cstddef>
#include <constant.h>
#include <stdio.h>
#include <stdlib.h>
#include "../core/Console.hpp"


using namespace std;

uint64_t PhysicalAllocator::FBT[] = {};
Page* PhysicalAllocator::page_array = nullptr;
uint64_t PhysicalAllocator::nb_physical_pages = 0;

int test_free_block(Page* page_array, uint64_t start, uint64_t end) {
	for (uint64_t i = start; i < end; i++) {
		if (page_array[i].type != PageType::FREE) {
			return 0;
		}
	}
	return 1;
}

void PhysicalAllocator::initAllocator(Page* page_array, uint64_t nb_physical_pages)
{
	PhysicalAllocator::page_array = page_array;
	PhysicalAllocator::nb_physical_pages = nb_physical_pages;
	for (int i=0; i<MAX_ORDER;i++) {
		FBT[i] = 0;
	}

	uint64_t i = 256;
	while (i < nb_physical_pages) {
		uint64_t tmp = i;
		int8_t max_order = 0;
		while (!(tmp & 0x1)) {
			tmp/=2;
			max_order++;
		}
		int8_t order = -1;
		uint64_t start = i;
		uint64_t end = i+1;
		while (order < max_order && test_free_block(page_array, start, end)) {
			order++;
			start = end;
			end += (1 << order);
		}

		if (order >= 0) {
			uint64_t *addr = page_array[i].kernelMappAddr;
			freePagesInternal(addr, order);
			i+=(1 << order);
		} else {
			i++;
		}
	}
}

uint64_t* PhysicalAllocator::fusion(uint64_t* addr, uint8_t order)
{
	uint64_t *buddy = calculAddrBuddy(addr, order);

	uint64_t *prev = &FBT[order];
	uint64_t *curr = (uint64_t*)FBT[order];

	/* Look for buddy bloc inside the free bloc list for the
	 * current order */
	while (curr < buddy && curr != nullptr) {
		// bloc already free
		if (curr == addr) return nullptr;
		prev = curr;
		curr = (uint64_t *) (*curr);
	}

	/* If the buddy is found, we remove the buddy from the free list
	 * and return the lowest address of the two blocs that now form one */
	if (curr == buddy) {
		*prev = *curr;
		if (addr < buddy)
			return addr;
		else
			return buddy;
	}

	/* If we reach the end of the list, we insert the new bloc inside it */
	*prev = (uint64_t)addr;
	*addr = (uint64_t)curr;
	return NULL;
}

uint64_t* PhysicalAllocator::calculAddrBuddy(uint64_t* addr, uint8_t order)
{
	uint64_t buddy = (uint64_t)addr;
	buddy = buddy ^ (PAGE_SIZE << order);
	return (uint64_t*)buddy;
}

uint64_t* PhysicalAllocator::allocPagesInternal(uint8_t order)
{
	uint8_t k0 = order;
	/** We look for a bloc big enough to provide the demanded size */
	while (FBT[k0] == 0) {
		k0++;
		/* If we reach the end of the FBT, we can't satisfy the allocation */
		if (k0 > MAX_ORDER)
			return nullptr;
	}
	/* Remove bloc from FBT */
	uint64_t *addr = (uint64_t *)FBT[k0];
	FBT[k0] = *addr;
	/* Fragment bloc until reaching desired size and save buddies in table */
	while (k0>order) {
		uint64_t* addr_buddy = calculAddrBuddy(addr, k0-1);
		k0--;
		FBT[k0]= (uintptr_t)addr_buddy;
		*addr_buddy = 0;
	}

	Page* page = getPageFromAddr(addr);
	for (uint64_t i = 0; i < (uint64_t)(1 << order); ++i) {
		page[i].type = PageType::ALLOCATED;
	}

	return addr;
}

Page* PhysicalAllocator::getPageFromAddr(uint64_t* addr) {
	if (addr == nullptr) {
		return nullptr;
	}
	uint64_t index = (uint64_t)addr & ~(KERNEL_START);
	index /= PAGE_SIZE;
	return page_array + index;
}

uint64_t* PhysicalAllocator::getAddrFromPage(Page* page) {
	return page->kernelMappAddr;
}

Page* PhysicalAllocator::allocPage()
{
	return allocPages(0);
}

Page* PhysicalAllocator::allocPages(uint8_t order) {
	uint64_t* addr = allocPagesInternal(order);
	return getPageFromAddr(addr);
}

Page* PhysicalAllocator::allocZeroedPage()
{
	return allocZeroedPages(0);
}

Page* PhysicalAllocator::allocZeroedPages(uint8_t order)
{
	Page* page = allocPages(order);
	uint64_t *addr = getAddrFromPage(page);
	for (uint64_t i=0; i < (uint64_t)((PAGE_SIZE/sizeof(uint64_t)) << order); i++) {
		addr[i] = 0;
	}
	return page;
}

int PhysicalAllocator::freePage(Page* page)
{
	return freePages(page, 0);
}

int PhysicalAllocator::freePages(Page* page, uint8_t order) {
	uint64_t* addr = getAddrFromPage(page);
	return freePagesInternal(addr, order);
}

int PhysicalAllocator::freePagesInternal(uint64_t* addr, uint8_t order)
{
	Page* page = getPageFromAddr(addr);
	for (uint64_t i = 0; i < (uint64_t)(1 << order); ++i) {
		page[i].type = PageType::FREE;
	}

	uint64_t* tmp = addr;
	/* Fusion of blocs when possible */
	while(tmp != NULL) {
		tmp = fusion(tmp, order);
		order++;
	}


	return 0;
}

void PhysicalAllocator::printStats() {
	uint64_t stats[5*2] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	for (uint64_t i = 0; i < PhysicalAllocator::nb_physical_pages; ++i) {
		stats[PhysicalAllocator::page_array[i].type] ++;
		if (i < 256) {
			stats[PhysicalAllocator::page_array[i].type + 5] ++;
		}
	}
	char str[512];
	Console::write("Memory (low_mem < 1Mo): pages (4Ko)\n");
	sprintf(str, "  Unavailable: %llu (low_mem : %llu)\n", stats[0], stats[5]);
	Console::write(str);
	sprintf(str, "  Free: %llu (low_mem : %llu)\n", stats[1], stats[6]);
	Console::write(str);
	sprintf(str, "  Allocated: %llu (low_mem : %llu)\n", stats[2], stats[7]);
	Console::write(str);
	sprintf(str, "  Kernel: %llu (low_mem : %llu)\n", stats[3], stats[8]);
	Console::write(str);
	sprintf(str, "  Cache: %llu\n", stats[4]);
	Console::write(str);
}


