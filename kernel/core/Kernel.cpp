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

#include <queue>

class TestElemHeap {
public:
	TestElemHeap(std::string name, int64_t prio): prio1(prio), name(name) {}

	TestElemHeap(const TestElemHeap& elem): prio1(elem.prio1), name(elem.name) {
	}

	int64_t prio1;
	std::string name;
};

class TestElemHeapCompare {
public:
	bool operator()( const TestElemHeap* lhs, const TestElemHeap* rhs ) const {
		return lhs->prio1 > rhs->prio1;
	}
};

bool compareTestElemHeap(const TestElemHeap* lhs, const TestElemHeap* rhs) {
	return lhs->prio1 > rhs->prio1;
}

typedef std::priority_queue<TestElemHeap*, std::vector<TestElemHeap*>,
		bool(*)(const TestElemHeap*, const TestElemHeap*)> TestHeap;

void Kernel::Start() {

	TestElemHeap* elem;
	TestElemHeap elem1(std::string("3"), 3);
	TestElemHeap elem2(std::string("20"), 20);
	TestElemHeap elem3(std::string("1"), 1);
	TestElemHeap elem4(std::string("10"), 10);
	TestElemHeap elem5(std::string("7"), 7);

	TestHeap heap(&compareTestElemHeap, std::vector<TestElemHeap*>());


	heap.push(&elem1);
	heap.push(&elem2);
	heap.push(&elem3);
	heap.push(&elem4);
	heap.push(&elem5);

	heap.remove(&elem3);

	elem = heap.top();
	Console::write(elem->name);
	Console::write(" < ");
	heap.pop();
	elem = heap.top();
	Console::write(elem->name);
	Console::write(" < ");
	heap.pop();
	elem = heap.top();
	Console::write(elem->name);
	Console::write(" < ");
	heap.pop();
	elem = heap.top();
	Console::write(elem->name);
	Console::write("\n");
	heap.pop();
	heap.pop();

	std::vector<int> vect;
	vect.push_back(2);

	auto it = vect.begin();
	int a = *it;

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

