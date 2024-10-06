/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/integral_constant.h>

namespace std {
template <class T>
struct is_const : std::false_type
{};
template <class T>
struct is_const<const T> : std::true_type
{};
template <class T>
constexpr bool is_const_v = is_const<T>::value;
}
