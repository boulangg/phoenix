/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __PAGE_HPP__
#define __PAGE_HPP__

#include <cstdint>

#include <include/constant.h>
class AddressSpace;

enum PageType
{
	UNUSABLE,
	FREE,
	ALLOCATED,
	KERNEL,
	CACHE
};

class Page
{
public:
	uint64_t* physAddr;
	uint64_t* kernelMappAddr;
	PageType type;
	uint64_t offset;
	AddressSpace* mapping;

	static void copyPage(Page* src, Page* dst)
	{
		for (uint64_t i = 0; i < PAGE_SIZE / sizeof(uint64_t); ++i) {
			dst->kernelMappAddr[i] = src->kernelMappAddr[i];
		}
	}
};

#endif // __PAGE_HPP__
