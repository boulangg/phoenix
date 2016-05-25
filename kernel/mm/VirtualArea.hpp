/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __VIRTUAL_AREA_HPP__
#define __VIRTUAL_AREA_HPP__

#include <cstddef>

#include "PhysicalMapping.hpp"
#include <fs/File.hpp>
#include <mm/Page.hpp>

class VirtualArea {
public:
	VirtualArea(uint64_t* addrStart, uint64_t* addrEnd, uint64_t flags,
			File* file, uint64_t offset, uint64_t fileSize);
	//VirtualArea(const VirtualArea&);
	~VirtualArea();

	uint64_t getNbPage();
	Page* getPage(uint64_t index);

	bool tryMergeArea(VirtualArea* area);


	enum FLAGS {
		VM_WRITE = 0x1,
		VM_READ = 0x2,
		VM_EXEC = 0x4,
		VM_MAYREAD = 0x8,
		VM_MAYWRITE = 0x01,	// Copy on write
		VM_MAYEXEC = 0x04,
		VM_SHARED = 0x40,
		VM_GROWSDOWN = 0x80,
		VM_GROWSUP = 0x100,
		VM_SHM = 0x200,
		VM_DENYWRITE = 0x400,
		VM_EXECUTABLE = 0x1000,
	};

	uint64_t* addrStart;
	uint64_t* addrEnd;
	uint64_t flags;
	uint64_t offset;
	uint64_t fileSize;

	// For anonymous mapping
	PhysicalMapping* physicalPages;

	// For file-backed mapping
	File* file;
};


#endif // __VIRTUAL_AREA_HPP__
