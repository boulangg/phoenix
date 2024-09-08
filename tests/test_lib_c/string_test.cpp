/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include "UnitTestFramework.h"

#include <cstring>

using namespace UnitTestFramework;

namespace libc {

TEST_CLASS(string)
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

    TEST_METHOD(memcpy_test)
    {
#define BUFFER_SIZE 1024
        char buffer[BUFFER_SIZE];
        memset(buffer, '2', BUFFER_SIZE);
        char buffer_cpy[BUFFER_SIZE];
        buffer_cpy[5] = '1';
        memcpy(buffer_cpy, buffer, BUFFER_SIZE);
        Assert::AreEqual('2', buffer[5]);
    }
};

}
