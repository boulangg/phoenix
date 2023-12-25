/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "UnitTestFramework.h"

#include <atomic>

#include <cstdint>

using namespace UnitTestFramework;

namespace libstdcpp {

TEST_CLASS(atomic_test)
{
public:
    TEST_METHOD(atomic_flag)
    {
        std::atomic_flag flag;
        Assert::IsFalse(flag.test());
        Assert::IsFalse(flag.test_and_set());

        Assert::IsTrue(flag.test());
        Assert::IsTrue(flag.test_and_set());

        flag.clear();
        Assert::IsFalse(flag.test());
    }
};

}
