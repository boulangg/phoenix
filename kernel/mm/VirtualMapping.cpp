/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "VirtualMapping.hpp"
#include <list>
#include <include/constant.h>
#include <iterator>


VirtualMapping::VirtualMapping(): pageTable(nullptr), entryPoint(nullptr),
	startCode(nullptr), endCode(nullptr), startData(nullptr), endData(nullptr),
	topStack(nullptr), startBrk(nullptr), currBrk(nullptr) {

}

PageTable* VirtualMapping::getPageTable() {
	if (pageTable == nullptr) {
		return reloadPageTable();
	} else {
		return pageTable;
	}
}

PageTable* VirtualMapping::reloadPageTable() {
	if (pageTable != nullptr) {
		delete pageTable;
	}
	pageTable = new PageTable(virtualAreas);
	return pageTable;
}

uint64_t* VirtualMapping::mmap(uint64_t* addr, uint64_t len, uint64_t prot,
		uint64_t flags, File* file, uint64_t offset) {
	return mmap(addr, len, prot, flags, file, offset, len);
}

uint64_t* VirtualMapping::mmap(uint64_t* addr, uint64_t len, uint64_t prot,
		uint64_t flags, File* file, uint64_t offset, uint64_t fileSize) {
	// TODO check if argument are correct.

	// TODO evaluate vmaFlags
	// WRITE + PRIVATE -> copy on write (MAY_WRITE)
	uint64_t vmaFlags = 0;
	if (prot & PROT::READ) {
		vmaFlags |= VirtualArea::FLAGS::VM_READ;
	}
	if ((prot & PROT::WRITE) && (flags & FLAGS::SHARED)) {
		vmaFlags |= VirtualArea::FLAGS::VM_WRITE;
	}
	if ((prot & PROT::WRITE) && (flags & FLAGS::PRIVATE)) {
		vmaFlags |= VirtualArea::FLAGS::VM_MAYWRITE;
	}
	if (prot & PROT::EXEC) {
		vmaFlags |= VirtualArea::FLAGS::VM_EXEC;
	}
	if (flags & FLAGS::SHARED) {
		vmaFlags |= VirtualArea::FLAGS::VM_SHARED;
	}
	if (flags & FLAGS::GROWNSDOWN) {
		vmaFlags |= VirtualArea::FLAGS::VM_GROWSDOWN;
	}
	if (flags & FLAGS::ANONYMOUS) {
		file = nullptr;
		offset = 0;
	}

	uint64_t* addrEnd = (uint64_t*)((uint64_t)addr+len);

	if (flags & FLAGS::FIXED) {
		VirtualArea* virtArea = new VirtualArea(addr, addrEnd, vmaFlags, file, offset, fileSize);
		virtArea = addVirtualArea(virtArea);

		return addr;
	}

	return 0;
}

uint64_t* VirtualMapping::findFreeVirtualArea(uint64_t* addr, uint64_t len) {
	uint64_t* freeAddr = addr;
	(void)len;
	for (auto it = virtualAreas.begin(); it != virtualAreas.end();++it) {
		// TODO
	}
	return freeAddr;
}

VirtualArea* VirtualMapping::addVirtualArea(VirtualArea* area) {
	auto it = virtualAreas.begin();
	for (; it != virtualAreas.end();++it) {
		if ((*it)->addrStart >= area->addrEnd) {
			break;
		}
	}

	if (it != virtualAreas.begin()) {
		auto prevIt = std::prev(it);
		if ((*prevIt)->addrEnd > area->addrStart) {
			delete area;
			return nullptr;
		}
	}

	it = virtualAreas.insert(it, area);
	it = mergeSurroundingAreas(it);
	return *it;
}

std::list<VirtualArea*>::iterator VirtualMapping::mergeSurroundingAreas(
		std::list<VirtualArea*>::iterator curr) {
	// Merge with previous one
	if (curr != virtualAreas.begin()) {
		auto prev = std::prev(curr);
		if ((*prev)->tryMergeArea(*curr)) {
			VirtualArea* old = *curr;
			virtualAreas.erase(curr);
			delete old;
			curr = prev;
		}
	}

	auto next = std::next(curr);
	// Merge with next one
	if (next != virtualAreas.end()) {
		if ((*curr)->tryMergeArea(*next)) {
			VirtualArea* old = *next;
			virtualAreas.erase(next);
			delete old;
		}
	}
	return curr;
}
