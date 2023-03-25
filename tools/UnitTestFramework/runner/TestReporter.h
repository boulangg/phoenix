#pragma once

#include <chrono>
#include <optional>
#include <unordered_map>

#include "UnitTestFramework.h"

#include "type.h"
#include "Utils.h"

using namespace UnitTestFramework;

namespace UnitTestRunner {

enum TestCaseStatus
{
	Success,
	Failure,
	Skipped
};

struct TestCaseErrorInfo
{
	std::string message;
};

struct TestCaseResult
{
	std::string name;
	TestCaseStatus status;
	std::chrono::nanoseconds time;
	std::optional<TestCaseErrorInfo> error;
};

struct TestSuiteResult
{
	std::string name;
	std::uint64_t failureCount;
	std::uint64_t successCount;
	std::uint64_t skippedCount;
	std::chrono::nanoseconds time;
	std::vector<TestCaseResult> testCases;
};

struct TestSuitesResult
{
	std::uint64_t failureCount;
	std::uint64_t successCount;
	std::uint64_t skippedCount;
	std::chrono::nanoseconds time;
	std::unordered_map<std::string, TestSuiteResult> testSuites;
};

class TestReporter
{
public:
	void ReportTestSuiteBegin(const TestSuiteMetadata& _metadata)
	{
		printf("### Begin Test Suite %s ###\n", demangle(_metadata.testSuite->typeInfo->name()).c_str());
	}

	void ReportTestSuiteEnd(const TestSuiteMetadata& _metadata, std::chrono::nanoseconds duration)
	{
		printf("### End Test Suite %s (%li ns) ###\n", demangle(_metadata.testSuite->typeInfo->name()).c_str(), std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
	}

	void ReportSuccess(const TestClassMetadata* classMetadata, const TestMethodMetadata* methodMetadata, std::chrono::nanoseconds duration)
	{
		(void)classMetadata;
		printf("  Success %s (%li ns)\n", methodMetadata->methodName.c_str(), std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
	}

	void ReportSkipped(const TestClassMetadata* classMetadata, const TestMethodMetadata* methodMetadata)
	{
		(void)classMetadata;
		printf("  Skipped %s\n", methodMetadata->methodName.c_str());
	}

	void ReportFailure(const TestClassMetadata* classMetadata, const TestMethodMetadata* methodMetadata, const TestFailureException& exc, std::chrono::nanoseconds duration)
	{
		(void)classMetadata;
		printf("  Failure %s (%li ns)\n", methodMetadata->methodName.c_str(), std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
		printf("    - %s\n", exc.getMessage().c_str());
	}

private:
	TestSuitesResult result;
};

}
