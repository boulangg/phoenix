#include "UnitTestFramework.h"

#include <unordered_map>

using namespace UnitTestFramework;

namespace stdlib {

TEST_CLASS(unoredered_map_test)
{
public:
	TEST_METHOD(unoredered_map_constructor_test)
	{
		std::unordered_map<std::uint64_t, std::uint64_t> map;
		auto value = map.find(10);
		(void)value;
	}
};

}
