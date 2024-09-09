/*
 * Copyright (c) 2016-2024 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "UnitTestFramework.h"

#include <unordered_map>

using namespace UnitTestFramework;

namespace libstdcpp {

TEST_CLASS(unordered_map_test)
{
public:
    TEST_METHOD(constructor)
    {
        std::unordered_map<std::uint64_t, std::uint64_t> map;
        auto findIt = map.find(10);
        Assert::IsTrue(map.end() == findIt);
    }

    TEST_METHOD(insert)
    {
        std::unordered_map<std::uint64_t, std::uint64_t> map;

        auto insertIt = map.insert(std::pair<std::uint64_t, std::uint64_t>(10, 20));
        Assert::IsTrue(insertIt.second);
        Assert::AreEqual<std::uint64_t>(10, insertIt.first->first);
        Assert::AreEqual<std::uint64_t>(20, insertIt.first->second);
        Assert::AreEqual<std::size_t>(1, map.size());

        Assert::IsTrue(map.contains(10));
        auto findIt = map.find(10);
        Assert::IsFalse(map.end() == findIt);
        Assert::AreEqual<std::uint64_t>(10, findIt->first);
        Assert::AreEqual<std::uint64_t>(20, findIt->second);

        auto insertIt2 = map.insert(std::pair<std::uint64_t, std::uint64_t>(10, 40));
        Assert::IsFalse(insertIt2.second);
        Assert::AreEqual<std::uint64_t>(10, insertIt2.first->first);
        Assert::AreEqual<std::uint64_t>(20, insertIt2.first->second);
        Assert::AreEqual<std::size_t>(1, map.size());

        Assert::IsTrue(map.contains(10));
        auto findIt2 = map.find(10);
        Assert::IsFalse(map.end() == findIt);
        Assert::AreEqual<std::uint64_t>(10, findIt2->first);
        Assert::AreEqual<std::uint64_t>(20, findIt2->second);
    }

    TEST_METHOD(erase)
    {
        std::unordered_map<std::uint64_t, std::uint64_t> map;

        map.insert(std::pair<std::uint64_t, std::uint64_t>(10, 20));
        map.insert(std::pair<std::uint64_t, std::uint64_t>(20, 40));

        Assert::IsTrue(map.contains(10));
        auto findIt = map.find(10);
        Assert::IsFalse(map.end() == findIt);
        Assert::AreEqual<std::size_t>(2, map.size());

        auto res = map.erase(10);

        Assert::IsTrue(res);
        Assert::IsFalse(map.contains(10));
        auto findIt2 = map.find(10);
        Assert::IsTrue(map.end() == findIt2);
        Assert::AreEqual<std::size_t>(1, map.size());

        auto res2 = map.erase(10);
        Assert::IsFalse(res2);
    }

    TEST_METHOD(clear)
    {
        std::unordered_map<std::uint64_t, std::uint64_t> map;

        map.insert(std::pair<std::uint64_t, std::uint64_t>(10, 20));
        map.insert(std::pair<std::uint64_t, std::uint64_t>(20, 40));

        Assert::AreEqual<std::size_t>(2, map.size());
        map.clear();
        Assert::AreEqual<std::size_t>(0, map.size());
    }

    TEST_METHOD(rehash)
    {
        std::unordered_map<std::uint64_t, std::uint64_t> map(2);

        Assert::IsTrue(map.load_factor() < map.max_load_factor());
        Assert::AreEqual<std::size_t>(2, map.bucket_count());

        map.insert(std::pair<std::uint64_t, std::uint64_t>(10, 20));
        map.insert(std::pair<std::uint64_t, std::uint64_t>(20, 30));
        map.insert(std::pair<std::uint64_t, std::uint64_t>(30, 40));
        map.insert(std::pair<std::uint64_t, std::uint64_t>(40, 50));
        map.insert(std::pair<std::uint64_t, std::uint64_t>(50, 60));

        Assert::IsTrue(map.load_factor() < map.max_load_factor());
        Assert::AreEqual<std::size_t>(8, map.bucket_count());

        map.max_load_factor(0.5);
        Assert::IsTrue(map.load_factor() < map.max_load_factor());
        Assert::AreEqual<std::size_t>(16, map.bucket_count());
    }

    TEST_METHOD(foreach)
    {
        std::unordered_map<std::uint64_t, std::uint64_t> map;
        for (auto& val : map) {
            (void)val;
            Assert::Fail("Map is supposed to be empty");
        }

        map.insert(std::pair<std::uint64_t, std::uint64_t>(10, 20));
        map.insert(std::pair<std::uint64_t, std::uint64_t>(20, 30));

        std::size_t count = 0;
        std::uint64_t sumKey = 0;
        std::uint64_t sumValue = 0;
        for (auto& val : map) {
            sumKey += val.first;
            sumValue += val.second;
            ++count;
        }
        Assert::AreEqual<std::size_t>(2, count);
        Assert::AreEqual<std::uint64_t>(30, sumKey);
        Assert::AreEqual<std::uint64_t>(50, sumValue);
    }

    TEST_METHOD(assignment)
    {
        std::unordered_map<std::uint64_t, std::uint64_t> map;
        map.insert(std::pair<std::uint64_t, std::uint64_t>(10, 20));
        Assert::IsTrue(map.contains(10));

        map = std::unordered_map<std::uint64_t, std::uint64_t>(11);
        Assert::IsFalse(map.contains(10));
    }
};

}
