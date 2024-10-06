/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/_enable_if.h>
#include <def/type_traits/type_categories.h>

namespace std {
template <class T, typename = typename std::enable_if<!std::is_object<T>::value, T*>::type>
constexpr T* addressof(T& arg) noexcept
{
    return &arg;
}
}
