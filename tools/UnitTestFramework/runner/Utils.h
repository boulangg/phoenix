#pragma once

#include <vector>

#include "UnitTestFramework.h"

using namespace UnitTestFramework;

namespace UnitTestRunner {

struct TestSuiteMetadata
{
	const TestClassMetadata* testSuite;
	std::vector<const TestMethodMetadata*> testCases;
};

}
