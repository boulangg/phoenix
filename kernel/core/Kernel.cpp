/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "Kernel.hpp"

#include <proc/ProcessScheduler.hpp>
#include <asm/cpu.h>

void Kernel::Start() {

	ProcessScheduler::init();
	idle();
	while(1)
		hlt();
	return;

}

