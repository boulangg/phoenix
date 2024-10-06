/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/references.h>

namespace std {
template <class T>
struct remove_cv
{
    typedef T type;
};
template <class T>
struct remove_cv<const T>
{
    typedef T type;
};
template <class T>
struct remove_cv<volatile T>
{
    typedef T type;
};
template <class T>
struct remove_cv<const volatile T>
{
    typedef T type;
};
template <class T>
using remove_cv_t = typename remove_cv<T>::type;

template <class T>
struct add_const
{
    typedef const T type;
};
template <class T>
using add_const_t = typename add_const<T>::type;

template <class T>
struct add_volatile
{
    typedef volatile T type;
};
template <class T>
using add_volatile_t = typename add_volatile<T>::type;

template <class T>
struct add_cv
{
    typedef const volatile T type;
};
template <class T>
using add_cv_t = typename add_cv<T>::type;
}
