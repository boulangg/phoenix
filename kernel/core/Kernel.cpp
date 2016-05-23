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

#include "Console.hpp"

#include <mm/PhysicalAllocator.hpp>


void Kernel::Start() {
	char str[512];
	sprintf(str, "0x%x, %i, %i\n", 152, 2, 3);
	Console::write("Hello world !!\n");
	Console::write(str);

	std::vector<char> vec;
	std::string stri("Hello how are U?\n");
	stri+="Fine\n";
	Console::write(stri);

	int* a = (int*)malloc(4096);


	asm("sti");
	asm("int $49");
	asm("cli");

	PhysicalAllocator::printStats();
}

