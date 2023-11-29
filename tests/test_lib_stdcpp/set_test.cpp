/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "UnitTestFramework.h"

#include <set>

using namespace UnitTestFramework;

namespace libstdcpp {

TEST_CLASS(set_test)
{
public:
    TEST_METHOD(constructor)
    {
        std::set<std::uint64_t> set;
        auto findIt = set.find(10);
        Assert::IsTrue(set.end() == findIt);
    }

    TEST_METHOD(insert)
    {
        std::set<std::uint64_t> set;

        auto insertIt = set.insert(10);
        Assert::IsTrue(insertIt.second);
        Assert::AreEqual<std::uint64_t>(10, *insertIt.first);
        Assert::AreEqual<std::size_t>(1, set.size());

        Assert::IsTrue(set.contains(10));
        auto findIt = set.find(10);
        Assert::IsFalse(set.end() == findIt);
        Assert::AreEqual<std::uint64_t>(10, *findIt);

        auto insertIt2 = set.insert(10);
        Assert::IsFalse(insertIt2.second);
        Assert::AreEqual<std::uint64_t>(10, *insertIt2.first);
        Assert::AreEqual<std::size_t>(1, set.size());
    }

    TEST_METHOD(erase)
    {
        std::set<std::uint64_t> set;

        set.insert(10);
        set.insert(20);

        Assert::IsTrue(set.contains(10));
        auto findIt = set.find(10);
        Assert::IsFalse(set.end() == findIt);
        Assert::AreEqual<std::size_t>(2, set.size());

        auto res = set.erase(10);

        Assert::IsTrue(res);
        Assert::IsFalse(set.contains(10));
        auto findIt2 = set.find(10);
        Assert::IsTrue(set.end() == findIt2);
        Assert::AreEqual<std::size_t>(1, set.size());

        auto res2 = set.erase(10);
        Assert::IsFalse(res2);
    }

    TEST_METHOD(clear)
    {
        std::set<std::uint64_t> set;

        set.insert(10);
        set.insert(20);

        Assert::AreEqual<std::size_t>(2, set.size());
        set.clear();
        Assert::AreEqual<std::size_t>(0, set.size());
    }

    TEST_METHOD(order)
    {
        std::set<std::uint64_t> set;

        set.insert(5);
        set.insert(2);
        set.insert(7);
        set.insert(3);
        set.insert(1);
        set.insert(4);
        set.insert(6);

        for (auto it = std::next(set.begin()); it != set.end(); it++) {
            auto prev = std::prev(it);
            Assert::IsTrue(*prev < *it, std::to_string(*it) + " <= " + std::to_string(*prev));
        }
    }
};

}
