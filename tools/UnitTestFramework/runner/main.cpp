/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>

#include "TestRunner.h"
#include "UnitTestFramework.h"

using namespace UnitTestFramework;

int main(int argc, const char* argv[])
{
	if (argc != 2) {
		printf("Expecting one argument with the dll name to test\n");
		abort();
	}

	UnitTestRunner::TestRunner testRunner(argv[1]);
	return testRunner.RunTests();
}
