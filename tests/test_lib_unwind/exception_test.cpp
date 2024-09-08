/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "UnitTestFramework.h"

#include <cstdint>

using namespace UnitTestFramework;

namespace libunwind {

TEST_CLASS(exception_test)
{
public:
    TEST_METHOD(try_catch_basic)
    {
        try {
            throw std::invalid_argument("test");
            Assert::Fail("UNREACHABLE: Should be throwing exception");
        } catch (std::invalid_argument& exc) {
            Assert::AreEqual<std::string>(exc.what(), "test");
        }
    }

    TEST_METHOD(try_catch_parent_exception)
    {
        try {
            throw std::invalid_argument("test");
            Assert::Fail("UNREACHABLE: Should be throwing exception");
        } catch (std::exception& exc) {
            Assert::AreEqual<std::string>(exc.what(), "test");
        }
    }

    void throwingMethod()
    {
        throw std::invalid_argument("test");
    }

    TEST_METHOD(try_catch_method_call)
    {
        try {
            throwingMethod();
            Assert::Fail("UNREACHABLE: Should be throwing exception");
        } catch (std::invalid_argument& exc) {
            Assert::AreEqual<std::string>(exc.what(), "test");
        }
    }

    TEST_METHOD(try_catch_rethrow)
    {
        try {
            try {
                throwingMethod();
                Assert::Fail("UNREACHABLE: Should be throwing exception");
            } catch (std::invalid_argument& exc) {
                Assert::AreEqual<std::string>(exc.what(), "test");
                throw;
                Assert::Fail("UNREACHABLE: Should be rethrowing exception");
            }
        } catch (std::invalid_argument& exc) {
            Assert::AreEqual<std::string>(exc.what(), "test");
        }
    }

    TEST_METHOD(try_catch_multiple_catch)
    {
        try {
            throwingMethod();
            Assert::Fail("UNREACHABLE: Should be throwing exception");
        } catch (std::domain_error& exc) {
            Assert::Fail("Unexpected error type");
        } catch (std::invalid_argument& exc) {
            Assert::AreEqual<std::string>(exc.what(), "test");
        }
    }
};

}
