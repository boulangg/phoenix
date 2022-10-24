#pragma once

#include <chrono>
#include <vector>

#include "UnitTestFramework.h"

#include "TestReporter.h"
#include "Utils.h"

using namespace UnitTestFramework;

namespace UnitTestRunner {

class TestSuiteInstance
{
public:
	TestSuiteInstance(const TestClassInfo* info) : _info(info), _instance(_info->_new())
	{}

	~TestSuiteInstance()
	{
		if (_instance != nullptr) {
			_info->_delete(_instance);
			_instance = nullptr;
		}
	}

	void Invoke(const TestMethodInfo* info)
	{
		_instance->Invoke(info->methodCallback);
	}

private:
	const TestClassInfo* _info;
	TestClassImpl* _instance;

};

class TestSuite
{
public:
	TestSuite(const TestClassInfo* info, std::vector<const TestMethodInfo*> testCases) :
		_info(info), _testCases(), _setUp(nullptr), _tearDown(nullptr), _metadata()
	{
		_metadata.testSuite = info->metadata;
		for (const TestMethodInfo* testCase : testCases) {
			switch (testCase->type) {
			case TestMethodInfoType::SetUp:
				_setUp = testCase;
				break;
			case TestMethodInfoType::TearDown:
				_setUp = testCase;
				_metadata.testCases.push_back(testCase->metadata);
				break;
			case TestMethodInfoType::TestMethod:
				_testCases.push_back(testCase);
				break;
			default:
				// TODO Log error ?
				break;
			}
		}
	}

	const TestSuiteMetadata& GetMetdata() const
	{
		return _metadata;
	}

	void RunTestSuite(TestReporter& reporter)
	{
		TestSuiteInstance instance(_info);

		reporter.ReportTestSuiteBegin(_metadata);
		auto startTestSuite = std::chrono::system_clock::now();

		for (const TestMethodInfo* testCase : _testCases) {
			auto start = std::chrono::system_clock::now();
			try {
				if (_setUp != nullptr) {
					instance.Invoke(_setUp);
				}
				instance.Invoke(testCase);
				if (_tearDown != nullptr) {
					instance.Invoke(_tearDown);
				}
				auto end = std::chrono::system_clock::now();
				reporter.ReportSuccess(_info->metadata, testCase->metadata, end - start);
			} catch (const TestFailureException& exc) {
				auto end = std::chrono::system_clock::now();
				reporter.ReportFailure(_info->metadata, testCase->metadata, exc, end - start);
			}
		}
		auto endTestSuite = std::chrono::system_clock::now();

		reporter.ReportTestSuiteEnd(_metadata, endTestSuite - startTestSuite);
	}

private:
	const TestClassInfo* _info;
	std::vector<const TestMethodInfo*> _testCases;
	const TestMethodInfo* _setUp;
	const TestMethodInfo* _tearDown;
	TestSuiteMetadata _metadata;
};

}
