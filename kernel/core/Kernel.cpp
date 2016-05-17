/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Kernel.hpp"

#include "Console.hpp"
#include <stdio.h>
#include <vector>

void Kernel::Start() {
	Console::initConsole();
	char str[512];
	sprintf(str, "0x%x, %i, %i\n", 152, 2, 3);
	Console::write("Hello world !!\n");
	Console::write(str);

    std::vector<char> vec;
    vec.push_back('H');
    vec.push_back('e');
    vec.push_back('l');
    vec.push_back('l');
    vec.push_back('o');
    vec.push_back(' ');
    vec.push_back('h');
    vec.push_back('o');
    vec.push_back('w');
    vec.push_back(' ');
    vec.push_back('a');
    vec.push_back('r');
    vec.push_back('e');
    vec.push_back(' ');
    vec.push_back('U');
    vec.push_back('?');
    vec.push_back('\n');
    for(unsigned int i=0;i<vec.size();i++)
        Console::write(vec[i]);

    unsigned int size = vec.size();
    for(unsigned int i=0;i<size;i++)
        vec.pop_back();
    vec.push_back('F');
    vec.push_back('i');
    vec.push_back('n');
    vec.push_back('e');
    vec.push_back('\n');
    for(unsigned int i=0;i<vec.size();i++)
        Console::write(vec[i]);

	int i = 0;
	(void)i;
	asm("sti");
	asm("int $49");
	asm("cli");
}

