/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstddef>

namespace std {
template <class T>
struct remove_extent
{
    using type = T;
};
template <class T>
struct remove_extent<T[]>
{
    using type = T;
};
template <class T, std::size_t N>
struct remove_extent<T[N]>
{
    using type = T;
};
template <class T>
using remove_extent_t = typename remove_extent<T>::type;

template <class T>
struct remove_all_extents
{
    typedef T type;
};
template <class T>
struct remove_all_extents<T[]>
{
    typedef typename remove_all_extents<T>::type type;
};
template <class T, std::size_t N>
struct remove_all_extents<T[N]>
{
    typedef typename remove_all_extents<T>::type type;
};
template <class T>
using remove_all_extents_t = typename remove_all_extents<T>::type;
}
