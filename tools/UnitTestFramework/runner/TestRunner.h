#pragma once

#include <unordered_map>
#include <vector>

#include "UnitTestFramework.h"
#include "TestSuite.h"

using namespace UnitTestFramework;

namespace UnitTestRunner {

class TestRunner
{
public:
	TestRunner(const std::string& path) : _path(path)
	{}

	void RunTests()
	{
		char* err = 0;

		void* dlHandle = dlopen(_path.c_str(), RTLD_NOW);
		err = dlerror();
		if (err != 0) {
			printf("%s", err);
			abort();
		}

		void* __start_TEST_SECTION_INIT_METADATA = dlsym(dlHandle, "__start_TEST_INIT");
		err = dlerror();
		if (err != 0) {
			printf("%s", err);
			abort();
		}

		void* __stop_TEST_SECTION_INIT_METADATA = dlsym(dlHandle, "__stop_TEST_INIT");
		err = dlerror();
		if (err != 0) {
			printf("%s", err);
			abort();
		}

		std::unordered_map<const TestClassInfo*, std::vector<const TestMethodInfo*>> testGroups;

		typedef TestMethodInfo* (*init_method)();
		init_method* iter = (init_method*)__start_TEST_SECTION_INIT_METADATA;
		for (; iter < (init_method*)__stop_TEST_SECTION_INIT_METADATA; ++iter) {
			auto res = (*iter)();
			testGroups[res->classInfo].push_back(res);
			/*auto typeInfo = res->classInfo->metadata->typeInfo;
			auto typeInfoStr = demangle(typeInfo->name());
			printf("%s, %s, %s, %s\n", res->classInfo->metadata->functionName.c_str(), res->metadata->methodName.c_str(), res->metadata->functionName.c_str(), typeInfoStr.c_str());

			TestClassImpl* instance = res->classInfo->_new();
			instance->Invoke(res->methodCallback);
			res->classInfo->_delete(instance);*/
		}

		TestReporter testReporter;

		for (auto& testGroup : testGroups) {
			TestSuite testSuite(testGroup.first, testGroup.second);
			testSuite.RunTestSuite(testReporter);
		}
	}

private:
	std::string _path;
};

}
