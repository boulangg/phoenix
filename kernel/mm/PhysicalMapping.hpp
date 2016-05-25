/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __PHYSICAL_MAPPING_HPP__
#define __PHYSICAL_MAPPING_HPP__

#include <cstddef>
#include "Page.hpp"

#include <vector>

class PhysicalMapping {
public:
	PhysicalMapping(uint64_t nb);
	~PhysicalMapping();

	uint64_t size();

	void setPage(uint64_t index, Page* physAddr);
	Page* getPage(uint64_t index);

	//void incrUsageCount();
	//void decrUsageCount();
	//int getUsageCount();

	PhysicalMapping* pushBack(PhysicalMapping*);

private:
	std::vector<Page*> array;
};

#endif // __PHYSICAL_MAPPING_HPP__
