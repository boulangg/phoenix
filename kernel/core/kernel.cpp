/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "kernel.hpp"

void kernel_main() {
	int i = 0;
	(void)i;
	asm("sti");
	asm("int $49");
	asm("cli");
}

