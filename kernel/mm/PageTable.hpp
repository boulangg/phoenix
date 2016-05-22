/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __PAGE_TABLE_HPP__
#define __PAGE_TABLE_HPP__

#define PAGE_PRESENT_MASK   0x1
#define PAGE_WRITE_MASK     0x2
#define PAGE_USER_MASK      0x4
#define PAGE_UWP_MASK       (PAGE_USER_MASK | PAGE_WRITE_MASK | PAGE_PRESENT_MASK)

#include <cstdint>

class PageTable {
public:
	PageTable(uint64_t* pml4t);

	void mapKernel(uint64_t kernelLimit);

	int mapPage(uint64_t* physAddr, uint64_t* virtAddr,
			uint16_t flags, uint16_t highLvlFlags = PAGE_UWP_MASK);

	void clearTable();

	static PageTable getKernelPageTable() {
		return PageTable(PageTable::kernelPML4T);
	}


private:
	static uint64_t* kernelPML4T;

	uint64_t* PML4T;
};

#endif // __PAGE_TABLE_HPP__
