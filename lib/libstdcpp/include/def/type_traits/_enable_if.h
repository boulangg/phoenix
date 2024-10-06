/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

namespace std {

template <bool B, class T = void>
struct enable_if
{};
template <class T>
struct enable_if<true, T>
{
    typedef T type;
};
template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;

}
