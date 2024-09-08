/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "UnitTestFramework.h"

#include <functional>

#include <cstdint>

using namespace UnitTestFramework;

namespace libstdcpp {

bool isZeroFunction(int input)
{
    return input == 0;
}

struct TestMemberFunction
{
    bool isZeroMemberFunction(int input)
    {
        return input == 0;
    }
};

struct TestFunctionObject
{
    bool operator()(int input) const
    {
        return input == 0;
    }
};

struct TestStaticMember
{
    static bool isZeroStaticMember(int input)
    {
        return input == 0;
    }
};

TEST_CLASS(function_test)
{
public:
    TEST_METHOD(lambda)
    {
        auto l = [](int input) -> bool { return input == 0; };
        std::function<bool(int)> isZero{l};
        Assert::IsTrue(isZero(0));
        Assert::IsFalse(isZero(10));
    }

    TEST_METHOD(function)
    {
        std::function<bool(int)> isZero = isZeroFunction;
        Assert::IsTrue(isZero(0));
        Assert::IsFalse(isZero(10));
    }

    TEST_METHOD(memberFunction)
    {
        auto test = &TestMemberFunction::isZeroMemberFunction;
        TestMemberFunction instance;
        (instance.*test)(10);

        std::function<bool(TestMemberFunction, int)> isZero(&TestMemberFunction::isZeroMemberFunction);
        Assert::IsTrue(isZero(instance, 0));
        Assert::IsFalse(isZero(instance, 10));
    }

    TEST_METHOD(memberFunctionWithLambda)
    {
        TestMemberFunction instance;
        auto l = [&](int input) -> bool { return instance.isZeroMemberFunction(input); };
        std::function<bool(int)> isZero = std::function<bool(int)>(l);
        Assert::IsTrue(isZero(0));
        Assert::IsFalse(isZero(10));
    }

    TEST_METHOD(functionObject)
    {
        std::function<bool(int)> isZero{TestFunctionObject()};
        Assert::IsTrue(isZero(0));
        Assert::IsFalse(isZero(10));
    }

    TEST_METHOD(staticMember)
    {
        std::function<bool(int)> isZero{&TestStaticMember::isZeroStaticMember};
        Assert::IsTrue(isZero(0));
        Assert::IsFalse(isZero(10));
    }

    // TEST_METHOD(bindMemberFucntionAndObject)
    //{
    //    TestFunctionObject instance;
    //    std::function<bool(int)> isZero = std::bind(&TestMemberFunction::isZeroMemberFunction, instance);
    //    Assert::IsTrue(isZero(0));
    //    Assert::IsFalse(isZero(10));
    //}

    // TEST_METHOD(bindMemberFucntionAndObjectPtr)
    //{
    //    TestFunctionObject instance;
    //    std::function<bool(int)> isZero = std::bind(&TestMemberFunction::isZeroMemberFunction, &instance);
    //    Assert::IsTrue(isZero(0));
    //    Assert::IsFalse(isZero(10));
    //}
};

}
