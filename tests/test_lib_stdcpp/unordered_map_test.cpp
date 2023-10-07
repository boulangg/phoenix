#include "UnitTestFramework.h"

#include <unordered_map>

using namespace UnitTestFramework;

namespace libstdcpp {

TEST_CLASS(unordered_map)
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

		auto insertIt = map.insert(std::pair<uint64_t, uint64_t>(10, 20));
		Assert::IsTrue(insertIt.second);
		Assert::AreEqual<std::uint64_t>(10, insertIt.first->first);
		Assert::AreEqual<std::uint64_t>(20, insertIt.first->second);
		Assert::AreEqual<std::size_t>(1, map.size());

		auto findIt = map.find(10);
		Assert::IsFalse(map.end() == findIt);
		Assert::AreEqual<std::uint64_t>(10, findIt->first);
		Assert::AreEqual<std::uint64_t>(20, findIt->second);

		auto insertIt2 = map.insert(std::pair<uint64_t, uint64_t>(10, 40));
		Assert::IsFalse(insertIt2.second);
		Assert::AreEqual<std::uint64_t>(10, insertIt2.first->first);
		Assert::AreEqual<std::uint64_t>(20, insertIt2.first->second);
		Assert::AreEqual<std::size_t>(1, map.size());

		auto findIt2 = map.find(10);
		Assert::IsFalse(map.end() == findIt);
		Assert::AreEqual<std::uint64_t>(10, findIt2->first);
		Assert::AreEqual<std::uint64_t>(20, findIt2->second);
	}

	TEST_METHOD(erase)
	{
		std::unordered_map<std::uint64_t, std::uint64_t> map;

		map.insert(std::pair<uint64_t, uint64_t>(10, 20));
		map.insert(std::pair<uint64_t, uint64_t>(20, 40));

		auto findIt = map.find(10);
		Assert::IsFalse(map.end() == findIt);
		Assert::AreEqual<std::size_t>(2, map.size());
		auto res = map.erase(10);
		Assert::IsTrue(res);
		auto findIt2 = map.find(10);
		Assert::IsTrue(map.end() == findIt2);
		Assert::AreEqual<std::size_t>(1, map.size());

		auto res2 = map.erase(10);
		Assert::IsFalse(res2);
	}

	TEST_METHOD(clear)
	{
		std::unordered_map<std::uint64_t, std::uint64_t> map;

		map.insert(std::pair<uint64_t, uint64_t>(10, 20));
		map.insert(std::pair<uint64_t, uint64_t>(20, 40));

		Assert::AreEqual<std::size_t>(2, map.size());
		map.clear();
		Assert::AreEqual<std::size_t>(0, map.size());
	}
};

}
