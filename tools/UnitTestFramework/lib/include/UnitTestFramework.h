#pragma once

#include "UnitTestFrameworkAssert.h"

#include <cstdint>
#include <string>
#include <typeinfo>

extern "C" const void* __start_TEST_INIT;
extern "C" const void* __stop_TEST_INIT;

namespace UnitTestFramework {

struct TestMethodMetadata
{
	std::string methodName;
	std::string functionName;
	std::string fileName;
	std::uint64_t lineNumber;
};

struct TestClassMetadata
{
	std::string functionName;
	std::string fileName;
	std::uint64_t lineNumber;
	const std::type_info* typeInfo;
};

enum TestMethodInfoType
{
	TestMethod,
	SetUp,
	TearDown
};

class TestClassImpl
{
public:
	typedef TestClassImpl* (*_newInstance)();
	typedef void (*_deleteInstance)(TestClassImpl*);
	typedef void (TestClassImpl::* __voidFunc)();

	virtual void Invoke(__voidFunc method) = 0;
	virtual ~TestClassImpl()
	{}
};

struct TestClassInfo
{
	TestClassImpl::_newInstance _new;
	TestClassImpl::_deleteInstance _delete;
	const TestClassMetadata* metadata;
};

struct TestMethodInfo
{
	TestMethodInfoType type;
	TestClassImpl::__voidFunc methodCallback;
	const TestMethodMetadata* metadata;
	const TestClassInfo* classInfo;
};

template<typename T>
class TestClass : public TestClassImpl
{
public:
	typedef T ThisClass;
	typedef void (ThisClass::* voidFunc)();

	static TestClassImpl* __New()
	{
		return new T();
	}

	static void __Delete(TestClassImpl* p)
	{
		delete p;
	}

	void Invoke(__voidFunc method) override
	{
		typedef void (ThisClass::* voidFunc2)();
		voidFunc2 method2 = static_cast<voidFunc2>(method);
		(static_cast<ThisClass*>(this)->*method2)();
	}

	static const TestClassInfo* __GetClassInfo() __attribute__((used))
	{
		__attribute__((section("TEST_CLASS_METADATA"))) static const TestClassMetadata metadata = { __PRETTY_FUNCTION__, __FILE__, __LINE__, &typeid(ThisClass) };
		__attribute__((section("TEST_CLASS_INFO"))) static const TestClassInfo info = { &__New, &__Delete, &metadata };
		return &info;
	}

	//static const std::type_info& typeInfo = typeid(ThisClass);

	static constexpr auto __start_TEST_INIT_internal __attribute__((used)) = &__start_TEST_INIT;
	static constexpr auto __stop_TEST_INIT_internal __attribute__((used)) = &__stop_TEST_INIT;
};

}

#define TEST_CLASS(TestSuiteName) class TestSuiteName : public UnitTestFramework::TestClass<TestSuiteName>

#define TEST_METHOD(TestName) \
	static const TestMethodInfo* __GetTestMethod_##TestName() __attribute__((used)) \
	{ \
		static constexpr auto __GetTestMethod_##TestName##_init __attribute__((used, section("TEST_INIT"))) = &ThisClass::__GetTestMethod_##TestName; \
		__attribute__((section("TEST_METHOD_METADATA"))) static const UnitTestFramework::TestMethodMetadata metadata = { #TestName, __PRETTY_FUNCTION__, __FILE__, __LINE__ }; \
		__attribute__((section("TEST_METHOD_INFO"))) static UnitTestFramework::TestMethodInfo info = { TestMethodInfoType::TestMethod, nullptr, &metadata, __GetClassInfo() }; \
		info.methodCallback = static_cast<UnitTestFramework::TestClassImpl::__voidFunc>(&ThisClass::TestName); \
		return &info; \
	} \
	void TestName()
