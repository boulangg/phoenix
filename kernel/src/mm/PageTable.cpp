/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "PageTable.hpp"

#include <include/constant.h>
#include <boot/processor_struct.hpp>

#include "PhysicalAllocator.hpp"

#include <mm/Page.hpp>


uint64_t* PageTable::kernelPML4T(kernel_pml4t);
uint64_t* PageTable::prevPML4T(kernel_pml4t);

PageTable::PageTable(uint64_t* pml4t) {
	this->PML4T = (uint64_t*)((uint64_t)pml4t & PAGE_ADDR_MASK);
}

PageTable::PageTable(std::list<VirtualArea*>& list) {
	this->PML4T = PhysicalAllocator::allocZeroedPage()->kernelMappAddr;
	copyKernelAddressSpace();
	for (auto it = list.begin(); it != list.end(); ++it) {
		mapUserVirtualArea(*it);
	}
}

void PageTable::mapUserVirtualArea(VirtualArea* area) {
	if (PML4T == nullptr) {
		return;
	}
	int index = 0;
	uint16_t flags = PAGE_UWP_MASK;
	if (area->flags & VirtualArea::FLAGS::VM_KERNEL) {
		flags = PAGE_KWP_MASK;
	}
	for (uint64_t addr = (uint64_t)(area->addrStart) & PAGE_ADDR_MASK; addr < (uint64_t)area->addrEnd; addr+=PAGE_SIZE) {
		uint64_t* virtAddr = (uint64_t*)addr;
		Page* physPage = area->getPage(index);
		mapPage(physPage->physAddr, virtAddr, flags);
		index++;
	}
}

int PageTable::mapPage(uint64_t* physAddr, uint64_t* virtAddr, uint16_t flags, uint16_t highLvlFlags) {
	if (PML4T == nullptr) {
		return -1;
	}
	uint64_t PML4T_index = ((uint64_t)virtAddr >> PML4E_INDEX_OFFSET) & PML4E_INDEX_MASK;
	if (PML4T[PML4T_index] == 0) {
		Page* page = PhysicalAllocator::allocZeroedPage();
		PML4T[PML4T_index] = (uint64_t)page->physAddr | highLvlFlags;
	}
	uint64_t* PDPT = (uint64_t*)((PML4T[PML4T_index] & PAGE_ADDR_MASK) | KERNEL_MAPPING_START);
	uint64_t PDPT_index = ((uint64_t)virtAddr >> PDPE_INDEX_OFFSET) & PDPE_INDEX_MASK;
	if (PDPT[PDPT_index] == 0) {
		Page* page = PhysicalAllocator::allocZeroedPage();
		PDPT[PDPT_index] = (uint64_t)page->physAddr | highLvlFlags;
	}
	uint64_t* PD = (uint64_t*)((PDPT[PDPT_index] & PAGE_ADDR_MASK) | KERNEL_MAPPING_START);
	uint64_t PD_index = ((uint64_t)virtAddr >> PDE_INDEX_OFFSET) & PDE_INDEX_MASK;
	if (PD[PD_index] == 0) {
		Page* page = PhysicalAllocator::allocZeroedPage();
		PD[PD_index] = (uint64_t)page->physAddr | highLvlFlags;
	}
	uint64_t* PT = (uint64_t*)((PD[PD_index] & PAGE_ADDR_MASK) | KERNEL_MAPPING_START);
	uint64_t PTE_index = ((uint64_t)virtAddr >> PTE_INDEX_OFFSET) & PTE_INDEX_MASK;

	uint64_t PTE = (uint64_t)physAddr & PAGE_ADDR_MASK;
	PT[PTE_index] = PTE | flags;

	return 0;
}
