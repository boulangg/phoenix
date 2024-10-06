/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/integral_constant.h>

namespace std {
template <class T, class U>
struct is_same : std::false_type
{};
template <class T>
struct is_same<T, T> : std::true_type
{};
template <class T, class U>
constexpr bool is_same_v = is_same<T, U>::value;
}