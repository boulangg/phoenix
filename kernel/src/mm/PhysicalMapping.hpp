/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __PHYSICAL_MAPPING_HPP__
#define __PHYSICAL_MAPPING_HPP__

#include <cstddef>
#include "Page.hpp"

#include <vector>
#include <include/constant.h>

class PhysicalMapping
{
public:
	PhysicalMapping(uint64_t nb);
	PhysicalMapping(const PhysicalMapping&);
	~PhysicalMapping();

	uint64_t size() const;

	void setPage(uint64_t index, Page* physAddr);
	Page* getPage(uint64_t index);

	PhysicalMapping* pushBack(PhysicalMapping*);

	void write(const char* src, int offset, int size);

	void read(const char* src, int offset, int size);


private:
	void writeOnPage(Page* pg, const char* src, int offset, int size)
	{
// TODO check validity of arguments
		char* dst = (char*)(pg->kernelMappAddr) + offset;
		for (int i = 0; i < size; ++i) {
			dst[i] = src[i];
		}
	}

	std::vector<Page*> array;
};

#endif // __PHYSICAL_MAPPING_HPP__
