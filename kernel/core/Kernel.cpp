/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Kernel.hpp"

#include "Console.hpp"
#include <stdio.h>

void Kernel::Start() {
	Console::initConsole();
	char str[512];
	sprintf(str, "0x%x, %i, %i\n", 152, 2, 3);
	Console::write("Hello world !!\n");
	Console::write(str);

	int i = 0;
	(void)i;
	asm("sti");
	asm("int $49");
	asm("cli");
}

