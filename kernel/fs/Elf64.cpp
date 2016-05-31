/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Elf64.hpp"

VirtualMapping* Elf64::getVirtualMapping(File* file) {
	VirtualMapping* virtualMap = new VirtualMapping();
	char* startAddr = file->getKernelStartAddr();
	Elf64_Ehdr *fileHeader = (Elf64_Ehdr*)startAddr;
	// TODO check if supported elf file
	//virtualMap->entryPoint = (uint64_t*)fileHeader->e_entry;
	char* programHeaderAddr = startAddr + fileHeader->e_phoff;
	for (uint32_t i = 0; i < fileHeader->e_phnum; ++i) {
		Elf64_Phdr *programHeader = (Elf64_Phdr*)programHeaderAddr;
		if (programHeader->p_type == programType::PT_LOAD) {
			uint64_t prot = VirtualMapping::PROT::NONE;
			uint64_t flags = VirtualMapping::FLAGS::PRIVATE |
					VirtualMapping::FLAGS::FIXED;
			if (programHeader->p_flags & programFlag::PF_R) {
				prot |= VirtualMapping::PROT::READ;
			}
			if (programHeader->p_flags & programFlag::PF_W) {
				prot |= VirtualMapping::PROT::WRITE;
				virtualMap->startData = (uint64_t*)(programHeader->p_vaddr);
				virtualMap->endData = (uint64_t*)(programHeader->p_vaddr+programHeader->p_memsz);
			}
			if (programHeader->p_flags & programFlag::PF_X) {
				prot |= VirtualMapping::PROT::EXEC;
				flags |= VirtualMapping::FLAGS::EXECUTABLE;
				virtualMap->startCode = (uint64_t*)(programHeader->p_vaddr);
				virtualMap->endCode = (uint64_t*)(programHeader->p_vaddr+programHeader->p_filesz);

			}

			uint64_t* addr = (uint64_t*)(programHeader->p_vaddr);
			uint64_t len = programHeader->p_memsz;
			uint64_t offset = programHeader->p_offset;
			virtualMap->mmap(addr, len, prot, flags, file, offset, programHeader->p_filesz);
		}

		programHeaderAddr += fileHeader->e_phentsize;
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

	virtualMap->topStack = (uint64_t*)(USER_STACK_END);

	virtualMap->setEntryPoint((uint64_t*)fileHeader->e_entry);

	virtualMap->mmap(addr, len, prot, flags, nullptr, 0, 0);
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

	virtualMap->startBrk = (uint64_t*)(USER_HEAP_START);
	virtualMap->currBrk = (uint64_t*)(USER_HEAP_START);

	virtualMap->mmap(addr, len, prot, flags, nullptr, 0, 0);
	}

	virtualMap->reloadPageTable();

	return virtualMap;

}

// Code for section parsing
/*
	char* sectionHeaderAddr = startAddr + fileHeader->e_shoff;
	for (int i = 0; i < fileHeader->e_shnum; i++) {
		Elf64_Shdr * sectionHeader = (Elf64_Shdr*)sectionHeaderAddr;
		uint64_t* addr;
		uint64_t len;
		uint64_t prot;
		uint64_t flags;
		uint64_t offset;
		if (sectionHeader->sh_flags & SHF_ALLOC) {
		switch (sectionHeader->sh_type) {
		case SHT_PROGBITS:
		case SHT_INIT_ARRAY:
		case SHT_FINI_ARRAY:
			addr = (uint64_t*)sectionHeader->sh_addr;
			len = sectionHeader->sh_size;
			prot = VirtualMapping::PROT::READ;
			flags = VirtualMapping::FLAGS::PRIVATE |
					VirtualMapping::FLAGS::FIXED;
			if (sectionHeader->sh_flags & SHF_WRITE) {
				prot |= VirtualMapping::PROT::WRITE;
			}
			if (sectionHeader->sh_flags & SHF_EXECINSTR) {
				prot |= VirtualMapping::PROT::EXEC;
				flags |= VirtualMapping::FLAGS::EXECUTABLE;
			}

			offset = sectionHeader->sh_offset;
			virtualMap->mmap(addr, len, prot, flags, file, offset);
			break;
		case SHT_NOBITS:
			addr = (uint64_t*)sectionHeader->sh_addr;
			len = sectionHeader->sh_size;
			prot = VirtualMapping::PROT::READ;
			flags = VirtualMapping::FLAGS::PRIVATE |
					VirtualMapping::FLAGS::FIXED;
			if (sectionHeader->sh_flags & SHF_WRITE) {
				prot |= VirtualMapping::PROT::WRITE;
				flags |= VirtualMapping::FLAGS::ANONYMOUS;
			}
			virtualMap->mmap(addr, len, prot, flags, nullptr, 0);
			break;
		default:
			break;
		}
		}

		sectionHeaderAddr += fileHeader->e_shentsize;
	}
 */
