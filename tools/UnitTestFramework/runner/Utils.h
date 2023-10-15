/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <vector>

#include "UnitTestFramework.h"
#include "type.h"

using namespace UnitTestFramework;

namespace UnitTestRunner {

struct TestSuiteMetadata
{
    const TestClassMetadata* testSuite;
    std::vector<const TestMethodMetadata*> testCases;

    std::string getName() const
    {
        return demangle(testSuite->typeInfo->name());
    }
};

}
