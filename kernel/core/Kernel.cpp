/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Kernel.hpp"

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <list>

#include "Console.hpp"

#include <mm/PhysicalAllocator.hpp>
#include <mm/VirtualMapping.hpp>
#include <fs/Elf64.hpp>
#include <fs/KernelFS.hpp>
#include <fs/File.hpp>


void Kernel::Start() {
	char str[512];
	sprintf(str, "0x%x, %i, %i\n", 152, 2, 3);
	Console::write("Hello world !!\n");
	Console::write(str);

	std::vector<char> vec;
	std::string stri("Hello how are U?\n");
	stri+="Fine\n";
	Console::write(stri);

	std::list<int> listInt;
	listInt.push_back(0);

	File* f;
	VirtualMapping* mapping;
	f = KernelFS::getUserApp("test1");
	mapping = Elf64::getVirtualMapping(f);
	PageTable* pgTable = mapping->getPageTable();
	f = KernelFS::getUserApp("test2");
	mapping = Elf64::getVirtualMapping(f);
	f = KernelFS::getUserApp("don't exist");
	(void)f;


	asm("sti");
	asm("int $49");
	asm("cli");

	PhysicalAllocator::printStats();
}

