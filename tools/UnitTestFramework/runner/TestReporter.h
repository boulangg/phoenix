/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <chrono>
#include <optional>
#include <unordered_map>

#include "UnitTestFramework.h"

#include "Utils.h"
#include "type.h"

using namespace UnitTestFramework;

using namespace std::literals::chrono_literals;

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
    std::string name = "";
    TestCaseStatus status;
    std::chrono::nanoseconds time = std::chrono::nanoseconds(0);
    std::optional<TestCaseErrorInfo> error;
};

struct TestSuiteResult
{
    std::string name = "";
    std::uint64_t successCount = 0;
    std::uint64_t failureCount = 0;
    std::uint64_t skippedCount = 0;
    std::chrono::nanoseconds time = std::chrono::nanoseconds(0);
    std::vector<TestCaseResult> testCases;

    void addTestCaseResult(TestCaseResult testCase)
    {
        testCases.push_back(testCase);
        switch (testCase.status) {
        case TestCaseStatus::Success:
            successCount++;
            break;
        case TestCaseStatus::Failure:
            failureCount++;
            break;
        case TestCaseStatus::Skipped:
            skippedCount++;
            break;
        }
    }
};

struct TestSuitesResult
{
    std::uint64_t successCount = 0;
    std::uint64_t failureCount = 0;
    std::uint64_t skippedCount = 0;
    std::chrono::nanoseconds time = std::chrono::nanoseconds(0);
    std::unordered_map<std::string, TestSuiteResult> testSuites;
};

class TestReporter
{
private:
    static float nanoToMilliSec(std::chrono::nanoseconds duration)
    {
        return duration.count() / 1000.0f;
    }

public:
    void ReportTestSuiteBegin(const TestSuiteMetadata& testSuiteMetadata)
    {
        auto testSuiteName = testSuiteMetadata.getName();
        _result.testSuites[testSuiteName] = TestSuiteResult();
        _result.testSuites[testSuiteName].name = testSuiteName;

        printf("\u2500\u252c\u2500 Starting Test Suite %s\n", testSuiteName.c_str());
    }

    void ReportTestSuiteEnd(const TestSuiteMetadata& testSuiteMetadata, std::chrono::nanoseconds duration)
    {
        auto testSuiteName = testSuiteMetadata.getName();
        auto& testSuiteResult = _result.testSuites[testSuiteName];
        testSuiteResult.time = duration;
        _result.successCount += testSuiteResult.successCount;
        _result.failureCount += testSuiteResult.failureCount;
        _result.skippedCount += testSuiteResult.skippedCount;
        _result.time += duration;

        printf(" \u2514\u2500 Finished in %.2f ms\n", nanoToMilliSec(duration));
    }

    void ReportSuccess(const TestSuiteMetadata& testSuiteMetadata, const TestClassMetadata* classMetadata,
                       const TestMethodMetadata* methodMetadata, std::chrono::nanoseconds duration)
    {
        auto testSuiteName = testSuiteMetadata.getName();
        auto& testSuiteResult = _result.testSuites[testSuiteName];
        auto& testCaseName = methodMetadata->methodName;
        testSuiteResult.addTestCaseResult(TestCaseResult{testCaseName, TestCaseStatus::Success, duration, {}});

        (void)classMetadata;
        printf(" \u251C\u2500 \u2713 Success %-20s (%.2f ms)\n", testCaseName, nanoToMilliSec(duration));
    }

    void ReportSkipped(const TestSuiteMetadata& testSuiteMetadata, const TestClassMetadata* classMetadata,
                       const TestMethodMetadata* methodMetadata)
    {
        auto testSuiteName = testSuiteMetadata.getName();
        auto& testSuiteResult = _result.testSuites[testSuiteName];
        auto& testCaseName = methodMetadata->methodName;
        testSuiteResult.addTestCaseResult(
            TestCaseResult{testCaseName, TestCaseStatus::Skipped, std::chrono::nanoseconds(), {}});

        (void)classMetadata;
        printf(" \u251C\u2500 ! Skipped %-20s\n", testCaseName);
    }

    void ReportFailure(const TestSuiteMetadata& testSuiteMetadata, const TestClassMetadata* classMetadata,
                       const TestMethodMetadata* methodMetadata, const TestFailureException& exc,
                       std::chrono::nanoseconds duration)
    {
        auto testSuiteName = testSuiteMetadata.getName();
        auto& testSuiteResult = _result.testSuites[testSuiteName];
        auto& testCaseName = methodMetadata->methodName;
        auto errorMessage = exc.getMessage();
        testSuiteResult.addTestCaseResult(
            TestCaseResult{testCaseName, TestCaseStatus::Failure, duration, TestCaseErrorInfo{errorMessage}});

        (void)classMetadata;
        printf(" \u251C\u2500 \u2715 Failure %-20s (%.2f ms)\n", testCaseName, nanoToMilliSec(duration));
        printf(" \u2502    - %s\n", errorMessage.c_str());
    }

    bool isSuccess() const
    {
        return _result.failureCount == 0;
    }

private:
    TestSuitesResult _result;
};

}
