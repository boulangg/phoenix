/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/references.h>

namespace std {
template <typename T>
typename std::add_rvalue_reference<T>::type declval() noexcept
{
    static_assert(false, "declval not allowed in an evaluated context");
}
}
