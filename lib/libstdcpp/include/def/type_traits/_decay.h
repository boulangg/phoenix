/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/references.h>
#include <def/type_traits/type_categories.h>
#include <def/type_traits/_conditional.h>
#include <def/type_traits/pointers.h>
#include <def/type_traits/arrays.h>

namespace std{
template <class T>
struct decay
{
private:
    typedef typename std::remove_reference<T>::type U;

public:
    typedef typename std::conditional<
        std::is_array<U>::value, typename std::add_pointer<typename std::remove_extent<U>::type>::type,
        typename std::conditional<std::is_function<U>::value, typename std::add_pointer<U>::type,
                                  typename std::remove_cv<U>::type>::type>::type type;
};
template <class T>
using decay_t = typename decay<T>::type;
}
