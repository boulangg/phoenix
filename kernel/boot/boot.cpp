/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "boot.h"

#include <asm/cpu.h>
#include <core/Kernel.hpp>
#include <core/Console.hpp>

#include "processor_struct.hpp"
#include "SetupProcessor.hpp"


void boot() {
	Console::initConsole();
	SetupProcessor::setupAll();
	Kernel::Start();
}

