/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "VirtualMapping.hpp"
#include <list>
#include <include/constant.h>
#include <iterator>
#include <cstring>

extern "C" void switch_to_user_mode();

VirtualMapping::VirtualMapping(): pageTable(nullptr), entryPoint(nullptr),
startCode(nullptr), endCode(nullptr), startData(nullptr), endData(nullptr),
topStack(nullptr), startStack(nullptr), startBrk(nullptr), currBrk(nullptr) {
	// Add syscall stack
	{
		uint64_t prot = VirtualMapping::PROT::EXEC
				| VirtualMapping::PROT::WRITE
				| VirtualMapping::PROT::READ;
		uint64_t flags = VirtualMapping::FLAGS::PRIVATE
				| VirtualMapping::FLAGS::ANONYMOUS
				| VirtualMapping::FLAGS::EXECUTABLE
				| VirtualMapping::FLAGS::FIXED
				| VirtualMapping::FLAGS::KERNEL;
		uint64_t* addr = (uint64_t*)(USER_SYSCALL_STACK_START);
		uint64_t len = (USER_SYSCALL_STACK_END-USER_SYSCALL_STACK_START);

		mmap(addr, len, prot, flags, nullptr, 0, 0);
	}

	// Add interrupt stack
	{
		uint64_t prot = VirtualMapping::PROT::EXEC
				| VirtualMapping::PROT::WRITE
				| VirtualMapping::PROT::READ;
		uint64_t flags = VirtualMapping::FLAGS::PRIVATE
				| VirtualMapping::FLAGS::ANONYMOUS
				| VirtualMapping::FLAGS::EXECUTABLE
				| VirtualMapping::FLAGS::FIXED
				| VirtualMapping::FLAGS::KERNEL;
		uint64_t* addr = (uint64_t*)(USER_INT_STACK_START);
		uint64_t len = (USER_INT_STACK_END-USER_INT_STACK_START);

		mmap(addr, len, prot, flags, nullptr, 0, 0);
	}

	// Add stack
	{
		uint64_t prot = VirtualMapping::PROT::EXEC
				| VirtualMapping::PROT::WRITE
				| VirtualMapping::PROT::READ;
		uint64_t flags = VirtualMapping::FLAGS::PRIVATE
				| VirtualMapping::FLAGS::ANONYMOUS
				| VirtualMapping::FLAGS::EXECUTABLE
				| VirtualMapping::FLAGS::GROWNSDOWN
				| VirtualMapping::FLAGS::FIXED;
		uint64_t* addr = (uint64_t*)(USER_STACK_START);
		uint64_t len = USER_STACK_END-USER_STACK_START;

		topStack = (uint64_t*)(USER_STACK_END);

		mmap(addr, len, prot, flags, nullptr, 0, 0);
	}

	// Add heap
	{
		uint64_t prot = VirtualMapping::PROT::EXEC
				| VirtualMapping::PROT::WRITE
				| VirtualMapping::PROT::READ;
		uint64_t flags = VirtualMapping::FLAGS::PRIVATE
				| VirtualMapping::FLAGS::ANONYMOUS
				| VirtualMapping::FLAGS::EXECUTABLE
				| VirtualMapping::FLAGS::FIXED;
		uint64_t* addr = (uint64_t*)(USER_HEAP_START);
		uint64_t len = (USER_HEAP_END-USER_HEAP_START);

		startBrk = (uint64_t*)(USER_HEAP_START);
		currBrk = (uint64_t*)(USER_HEAP_START);

		mmap(addr, len, prot, flags, nullptr, 0, 0);
	}
}

VirtualMapping::VirtualMapping(const VirtualMapping& mapping) : virtualAreas() {
	pageTable = nullptr;
	entryPoint = mapping.entryPoint;
	startCode = mapping.startCode;
	endCode = mapping.endCode;
	startData = mapping.startData;
	endData = mapping.endData;
	topStack = mapping.topStack;
	startStack = mapping.startStack;
	startBrk = mapping.startBrk;
	currBrk = mapping.currBrk;
	VirtualMapping* mapp = (VirtualMapping*)&mapping;
	for (auto it = mapp->virtualAreas.begin(); it != mapp->virtualAreas.end(); ++it) {
		VirtualArea* virtArea = new VirtualArea(**it);
		addVirtualArea(virtArea);
	}
}

VirtualMapping::~VirtualMapping() {

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

/*typedef struct
{
	int a_type;
	union {
		long a_val;
		void *a_ptr;
		void (*a_fnc)();
	} a_un;
} auxv_t;*/

void VirtualMapping::initMainArgs(const char*argv[], const char*envp[], bool switchToUserMode) {
	size_t argc = 0;
	size_t argSize = 0;
	while (argv[argc] != nullptr) {
		argSize += strlen(argv[argc]) + 1;
		++argc;
	}
	size_t envc = 0;
	size_t envSize = 0;
	while(envp[envc]) {
		envSize += strlen(envp[envc]) + 1;
		++envc;
	}

	size_t totalSize = (1 + argc + 1 + envc + 1 + 1)*sizeof(uint64_t) + argSize + envSize;
	totalSize  = (totalSize+sizeof(uint64_t)-1) / sizeof(uint64_t);

	PageTable::setActivePageTable(getPageTable());

	startStack = topStack - totalSize - 1;

	startStack[0] = (uint64_t)entryPoint;
	startStack[1] = argc;

	char* infoStart = (char*)(startStack + 4 + argc + envc + 1);
	for (size_t i = 0; i < argc; ++i) {
		startStack[2+i] = (uint64_t)strcpy(infoStart, argv[i]);
		infoStart += strlen(argv[i]);
	}
	startStack[2+argc] = 0;      // Null pointer to end argv
	for (size_t i = 0; i < envc; ++i) {
		startStack[3+argc+i] = (uint64_t)strcpy(infoStart, envp[i]);
		infoStart += strlen(argv[i]);
	}
	startStack[3+argc+envc] = 0; // Null pointer to end envp
	startStack[4+argc+envc] = 0;	// Null auxiliary vector

	if (switchToUserMode) {
		startStack --;
		startStack[0] = (uint64_t)switch_to_user_mode;
	}

	PageTable::restorePreviousPageTable();
}

void VirtualMapping::setEntryPoint(uint64_t* entryPoint) {
	if (startStack == nullptr) {
		startStack = topStack-1;
	}
	//startStack[0] = (uint64_t)entryPoint;
	this->entryPoint = entryPoint;
}

uint64_t* VirtualMapping::mmap(uint64_t* addr, uint64_t len, uint64_t prot,
		uint64_t flags, File* file, uint64_t offset) {
	return mmap(addr, len, prot, flags, file, offset, len);
}

uint64_t* VirtualMapping::mmap(uint64_t* addr, uint64_t len, uint64_t prot,
		uint64_t flags, File* file, uint64_t offset, uint64_t fileSize) {
	// TODO check if argument are correct.

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
	if (flags & FLAGS::KERNEL) {
		vmaFlags |= VirtualArea::FLAGS::VM_KERNEL;
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
