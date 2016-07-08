/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <mm/VirtualArea.hpp>
#include <include/constant.h>
#include <algorithm>

VirtualArea::VirtualArea(uint64_t* addrStart, uint64_t* addrEnd, uint64_t flags,
		File* file, uint64_t offset, uint64_t fileSize) {
	this->addrStart = addrStart;
	this->addrEnd = addrEnd;
	this->flags = flags;
	this->file = file;
	this->offset = offset;
	this->fileSize = fileSize;
	uint64_t nbPages = (((uint64_t)addrEnd)+PAGE_SIZE-1)/PAGE_SIZE
			- ((uint64_t)addrStart)/PAGE_SIZE;
	if ((flags & FLAGS::VM_WRITE)
			&& !(flags & FLAGS::VM_SHARED) ) {
		physicalPages = new PhysicalMapping(nbPages);
	} else {
		physicalPages = nullptr;
	}
}

VirtualArea::VirtualArea(const VirtualArea& virtArea) {
	addrStart = virtArea.addrStart;
	addrEnd = virtArea.addrEnd;
	flags = virtArea.flags;
	file = virtArea.file;
	offset = virtArea.offset;
	fileSize = virtArea.fileSize;
	if (virtArea.physicalPages != nullptr) {
		physicalPages = new PhysicalMapping(*(virtArea.physicalPages));
	}
}

VirtualArea::~VirtualArea() {
	delete this->physicalPages;
}

size_t VirtualArea::getNbPages() {
	return (((uint64_t)addrEnd)+PAGE_SIZE-1)/PAGE_SIZE
			- ((uint64_t)addrStart)/PAGE_SIZE;
}

Page* VirtualArea::getPage(uint64_t index) {
	if (physicalPages != nullptr) {
		Page* page = physicalPages->getPage(index);
		if (page == nullptr) {
			page = PhysicalAllocator::allocZeroedPage();
			physicalPages->setPage(index, page);
			// Copy page if it comes from a file
			if (file != nullptr) {
				int64_t startPageOffset = (offset/PAGE_SIZE + index)*PAGE_SIZE;
				int64_t startOffsetLimit = (int64_t)offset-startPageOffset;
				int64_t startCpy = std::max(startOffsetLimit, (int64_t)0);

				int64_t endOffsetLimit = startOffsetLimit + fileSize;
				int64_t endCpy = std::min((int64_t)PAGE_SIZE, endOffsetLimit);


				int fileIndex = offset/PAGE_SIZE + index;
				Page* filePage = file->getPage(fileIndex);
				for (int64_t i = startCpy; i < endCpy; ++i) {
					((char*)(page->kernelMappAddr))[i] = ((char*)(filePage->kernelMappAddr))[i];
				}
			}
		}
		return page;
	} else {
		int fileIndex = offset/PAGE_SIZE + index;
		return file->getPage(fileIndex);
	}
}


bool VirtualArea::tryMergeArea(VirtualArea* area) {
	if (this->flags != area->flags) {
		return false;
	}
	if ((this->addrEnd != area->addrStart)
			& (this->file == area->file)){
		return false;
	}
	if (this->file == nullptr) {
		this->addrEnd = area->addrEnd;
		this->physicalPages->pushBack(area->physicalPages);
		return true;
	}
	if ((offset + (uint64_t)addrEnd - (uint64_t)addrStart) == area->offset) {
		this->addrEnd = area->addrEnd;
		return true;
	}
	return false;
}
