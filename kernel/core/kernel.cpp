/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "kernel.hpp"

#include "console.hpp"

void kernel_main() {
	Console::initConsole();
	Console::write("Hello world !!\n");

	int i = 0;
	(void)i;
	asm("sti");
	asm("int $49");
	asm("cli");
}

