#include "UnitTestFramework.h"

#include <cstring>

using namespace UnitTestFramework;

namespace stdlib {

TEST_CLASS(string_test)
{
public:
	TEST_METHOD(memset_test)
	{
#define BUFFER_SIZE 1024
		char buffer[BUFFER_SIZE];
		buffer[5] = '1';
		memset(buffer, '2', BUFFER_SIZE);
		Assert::AreEqual('2', buffer[5]);
	}

	TEST_METHOD(test_bis)
	{
		Assert::AreEqual(1, 2);
	}
};

}
