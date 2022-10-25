#pragma once

#include "UnitTestFramework.h"
#include "TestReporter.h"

using namespace UnitTestFramework;

namespace UnitTestRunner {

class GlobalStatusReporter
{
public:
	void ReportTestSuiteBegin(const TestSuiteMetadata& _metadata);
	void ReportTestSuiteEnd(const TestSuiteMetadata& _metadata);
	void ReportSuccess(const TestClassMetadata* classMetadata, const TestMethodMetadata* methodMetadata);
	void ReportFailure(const TestClassMetadata* classMetadata, const TestMethodMetadata* methodMetadata, const TestFailureException& exc);
};

}