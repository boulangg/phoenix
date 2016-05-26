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
#include <functional>

#include "Console.hpp"

#include <mm/PhysicalAllocator.hpp>
#include <mm/VirtualMapping.hpp>
#include <fs/Elf64.hpp>
#include <fs/KernelFS.hpp>
#include <fs/File.hpp>

#include <lib/Heap.hpp>

class TestElemHeap {
public:
	TestElemHeap(std::string name = ""): indexPrio1(0), prio1(0), indexPrio2(0), prio2(0)
		, name(name) {}

	TestElemHeap(const TestElemHeap& elem): indexPrio1(elem.indexPrio1),
			prio1(elem.prio1), indexPrio2(elem.prio2), prio2(elem.prio2),
			name(elem.name) {
	}


	TestElemHeap& operator=(const TestElemHeap& elem) {
		indexPrio1 = elem.indexPrio1;
		prio1 = elem.prio1;
		indexPrio2 = elem.indexPrio2;
		prio2 = elem.prio2;
		name = elem.name;

		return *this;
	}

	bool operator ==(const TestElemHeap& elem) {
		return name.compare(elem.name) == 0;
	}

	int64_t indexPrio1;
	int64_t prio1;
	int64_t indexPrio2;
	uint64_t prio2;
	std::string name;
};

typedef Heap<TestElemHeap, int64_t, &TestElemHeap::prio1, std::greater<int64_t>> TestHeap1;

void Kernel::Start() {

	TestHeap1 heap1;
	TestElemHeap* elem;
	TestElemHeap elem1(std::string("3"));
	TestElemHeap elem2(std::string("20"));
	TestElemHeap elem3(std::string("1"));
	TestElemHeap elem4(std::string("10"));

	heap1.insert(&elem1, 3);
	heap1.insert(&elem2, 5);
	heap1.insert(&elem3, 1);
	heap1.insert(&elem4, 10);

	heap1.update(&elem2, 20);

	elem = heap1.top();
	Console::write(elem->name);
	Console::write(" < ");
	heap1.pop();
	elem = heap1.top();
	Console::write(elem->name);
	Console::write(" < ");
	heap1.pop();
	elem = heap1.top();
	Console::write(elem->name);
	Console::write(" < ");
	heap1.pop();
	elem = heap1.top();
	Console::write(elem->name);
	Console::write("\n");
	heap1.pop();
	heap1.pop();


	File* f;
	VirtualMapping* mapping;
	f = KernelFS::getUserApp("test1");
	mapping = Elf64::getVirtualMapping(f);
	PageTable* pgTable = mapping->getPageTable();
	PageTable::setActivePageTable(pgTable);

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


	asm("sti");
	asm("int $49");
	asm("cli");

	PhysicalAllocator::printStats();
}

