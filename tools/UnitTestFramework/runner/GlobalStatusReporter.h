/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include "TestReporter.h"
#include "UnitTestFramework.h"

using namespace UnitTestFramework;

namespace UnitTestRunner {

class GlobalStatusReporter
{
public:
    void ReportTestSuiteBegin(const TestSuiteMetadata& _metadata);
    void ReportTestSuiteEnd(const TestSuiteMetadata& _metadata);
    void ReportSuccess(const TestClassMetadata* classMetadata, const TestMethodMetadata* methodMetadata);
    void ReportFailure(const TestClassMetadata* classMetadata, const TestMethodMetadata* methodMetadata,
                       const TestFailureException& exc);
};

}
