/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <string>
#include <typeinfo>

std::string demangle(const char* name);

template <class T>
std::string type(const T& t)
{
    return demangle(typeid(t).name());
}
