/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "sbrk.hpp"

#include <stddef.h>
#include <errno.h>

#include <constant.h>
#include "PhysicalAllocator.hpp"
#include "PageTable.hpp"

static char* kernel_sbrk_curr = (char*)KERNEL_HEAP_START;
static char* kernel_max_heap_curr = (char*)KERNEL_HEAP_START;


void *sbrk(ptrdiff_t diff)
{
	char* old_brk = kernel_sbrk_curr;
	char* new_brk = old_brk+diff;
	if ((new_brk > (char*)KERNEL_HEAP_END) || (new_brk < old_brk)) {
		errno = ENOMEM;
		return (void*)(-1);
	}
	while (new_brk > kernel_max_heap_curr) {
		Page* new_page = PhysicalAllocator::allocZeroedPage();
		PageTable kernelPageTable = PageTable::getKernelPageTable();
		kernelPageTable.mapPage(new_page->physAddr, (uint64_t*)kernel_max_heap_curr, 0x3, 0x3);
		kernel_max_heap_curr+=PAGE_SIZE;
	}
	return old_brk;
}
