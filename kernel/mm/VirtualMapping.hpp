/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef VIRTUALMEMORYMAPPING_HPP
#define VIRTUALMEMORYMAPPING_HPP

#include <cstddef>
#include <cstdint>

#include <list>
#include <mm/VirtualArea.hpp>
#include <mm/PageTable.hpp>

class VirtualMapping {

public:

	VirtualMapping();
	//VirtualMapping(const VirtualMapping& );

	~VirtualMapping();

	//void addVirtualArea(VirtualArea&);

	// Protection on pages
	enum PROT {
		NONE = 0x0,
		READ = 0x1,
		WRITE = 0x2,
		EXEC = 0x4
	};

	enum FLAGS {
		SHARED = 0x1,
		PRIVATE = 0x2,
		FIXED = 0x4,
		ANONYMOUS = 0x8,
		GROWNSDOWN = 0x10,
		DENYWRITE = 0x20,
		EXECUTABLE = 0x40,
		//LOCKED = 0x80,
		//NORESERVE = 0x100, // no swap space reserved
		//POPULATE = 0x200,
		//NONBLOCK = 0x400
	};

	void updatePageTable(VirtualArea*);

	uint64_t* mmap(uint64_t* addr, uint64_t len, uint64_t prot,
			uint64_t flags, File* file, uint64_t offset);
	uint64_t* mmap(uint64_t* addr, uint64_t len, uint64_t prot,
			uint64_t flags, File* file, uint64_t offset, uint64_t fileSize);

	int munmap(uint64_t* vAddr, size_t length);


	PageTable* reloadPageTable();
	PageTable* getPageTable();

private:
	// Find first virtualMemoryArea whose end is greater then addr
	std::list<VirtualArea*>::iterator findVirtualArea(uint64_t* addr);

	/* Add a virtual area to the mapping. If it is not possible to add
	 * it, nothing happens. In any case, the parameter became invalid.
	 * return nullptr if not added (the parameter is delete), virtual
	 * area that contain the parameter (there might have been fusion).
	 */
	VirtualArea* addVirtualArea(VirtualArea* area);

	/* Try to merge the area with the surrounding
	 * return the area containing the parameter
	 */
	std::list<VirtualArea*>::iterator mergeSurroundingAreas(
			std::list<VirtualArea*>::iterator currIt);

	uint64_t* findFreeVirtualArea(uint64_t* addr, uint64_t len);

	std::list<VirtualArea*> virtualAreas;
	PageTable* pageTable;

	//uint32_t user_count;
public:
	uint64_t* entryPoint;
	uint64_t* startCode;
	uint64_t* endCode;
	uint64_t* startData;
	uint64_t* endData;
	uint64_t* topStack;
	uint64_t* startBrk;
	uint64_t* currBrk;

};

#endif
