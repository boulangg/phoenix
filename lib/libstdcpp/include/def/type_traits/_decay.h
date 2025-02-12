/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <def/type_traits/_conditional.h>
#include <def/type_traits/arrays.h>
#include <def/type_traits/pointers.h>
#include <def/type_traits/references.h>
#include <def/type_traits/type_categories.h>

namespace std {
template <class T>
struct decay
{
private:
    using U = typename std::remove_reference<T>::type;

public:
    using type =
        std::conditional_t<std::is_array_v<U>, std::add_pointer_t<std::remove_extent_t<U>>,
                           std::conditional_t<std::is_function_v<U>, std::add_pointer_t<U>, std::remove_cv_t<U>>>;
};
template <class T>
using decay_t = typename decay<T>::type;
}
