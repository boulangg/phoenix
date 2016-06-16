/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <mm/PhysicalMapping.hpp>
#include <mm/PhysicalAllocator.hpp>

PhysicalMapping::PhysicalMapping(uint64_t nb) {
	Page* nullPage = nullptr;
	for (uint64_t i = 0; i < nb; ++i) {
		array.push_back(nullPage);
	}
}

PhysicalMapping::PhysicalMapping(const PhysicalMapping& mapping) {
	for (uint64_t i = 0; i < mapping.size(); ++i) {
		if (mapping.array[i] != nullptr) {
			Page* newPage = PhysicalAllocator::allocPage();
			Page::copyPage(mapping.array[i], newPage);
			array.push_back(newPage);
		}
	}
}


PhysicalMapping::~PhysicalMapping() {

}

uint64_t PhysicalMapping::size() const {
	return array.size();
}

void PhysicalMapping::setPage(uint64_t index, Page* physAddr) {
	array[index] = physAddr;
}

Page* PhysicalMapping::getPage(uint64_t index) {
	return array[index];
}

PhysicalMapping* PhysicalMapping::pushBack(PhysicalMapping* map) {
	for (uint64_t i = 0; i < map->size(); ++i) {
		array.push_back(array[i]);
	}
	return this;
}

