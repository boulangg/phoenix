/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstdint>

class AddressSpace;

struct Page
{
    enum Type
    {
        UNUSABLE,
        FREE,
        ALLOCATED,
        KERNEL,
        CACHE
    };

	uint64_t* physAddr;
	uint64_t* kernelMappAddr;
	Type type;
	uint64_t offset;
	AddressSpace* mapping;

	static void copyPage(Page* src, Page* dst)
	{
		for (uint64_t i = 0; i < PAGE_SIZE / sizeof(uint64_t); ++i) {
			dst->kernelMappAddr[i] = src->kernelMappAddr[i];
		}
	}
};

