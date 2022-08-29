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
#define PAGE_UROP_MASK      (PAGE_USER_MASK | PAGE_PRESENT_MASK)
#define PAGE_KWP_MASK		(PAGE_WRITE_MASK | PAGE_PRESENT_MASK)

#include <cstdint>
#include <list>
#include <mm/VirtualArea.hpp>
#include <boot/SetupProcessor.hpp>
#include <include/constant.h>

class PageTable
{
public:
	PageTable(std::list<VirtualArea*>& list);

	void mapUserVirtualArea(VirtualArea* area);

	int mapPage(uint64_t* physAddr, uint64_t* virtAddr,
				uint16_t flags, uint16_t highLvlFlags = PAGE_UWP_MASK);

	void clearTable();

	static PageTable getKernelPageTable()
	{
		return PageTable(PageTable::kernelPML4T);
	}

	uint64_t getPageTablePtr()
	{
		return (uint64_t)PML4T & ~(KERNEL_MAPPING_START);
	}

private:
	PageTable(uint64_t* pml4t);

	void copyKernelAddressSpace()
	{
		for (int i = 256; i < 512; ++i) {
			this->PML4T[i] = PageTable::kernelPML4T[i];
		}
	}

	/*uint64_t* getPhysicalPage(uint64_t* virtAddr) {
		return nullptr;
	}*/

	static uint64_t* kernelPML4T;
	static uint64_t* prevPML4T;

	uint64_t* PML4T;
};

#endif // __PAGE_TABLE_HPP__
