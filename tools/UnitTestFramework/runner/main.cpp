#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>

#include "UnitTestFramework.h"
#include "TestRunner.h"

using namespace UnitTestFramework;

int main(int argc, const char* argv[])
{
	if (argc != 1) {
		printf("Expecting one argument with the dll name to test\n");
		abort();
	}
	UnitTestRunner::TestRunner testRunner(argv[1]);
	testRunner.RunTests();
}
