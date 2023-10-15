/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <exception>

#include <stdlib.h>

using namespace std;

exception::exception() noexcept : _msg("") {}

exception::exception(const exception& exc) noexcept : _msg(exc._msg) {}

exception& exception::operator=(const exception& exc) noexcept
{
    _msg = exc._msg;
    return *this;
}

exception::~exception() {}

const char* exception::what() const noexcept
{
    return _msg;
}

void std::terminate() noexcept
{
    abort();
}
