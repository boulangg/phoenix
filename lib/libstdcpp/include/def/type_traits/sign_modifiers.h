/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/const_volatility_specifiers.h>
#include <def/type_traits/type_categories.h>

namespace std {
// TODO implem: make_signed for enum
namespace details {
template <class T, class U>
struct __copy_cv
{
    typedef U type;
};
template <class T, class U>
struct __copy_cv<const T, U>
{
    typedef const std::remove_cv_t<U> type;
};
template <class T, class U>
struct __copy_cv<volatile T, U>
{
    typedef volatile std::remove_cv_t<U> type;
};
template <class T, class U>
struct __copy_cv<const volatile T, U>
{
    typedef const volatile std::remove_cv_t<U> type;
};

template <class T, bool = std::is_integral_v<T> || std::is_enum_v<T>>
struct __make_signed{};

template <>
struct __make_signed<char, true>
{
    typedef signed char type;
};
template <>
struct __make_signed<signed char, true>
{
    typedef signed char type;
};
template <>
struct __make_signed<unsigned char, true>
{
    typedef signed char type;
};
template <>
struct __make_signed<short, true>
{
    typedef short type;
};
template <>
struct __make_signed<unsigned short, true>
{
    typedef short type;
};
template <>
struct __make_signed<int, true>
{
    typedef int type;
};
template <>
struct __make_signed<unsigned int, true>
{
    typedef int type;
};
template <>
struct __make_signed<long, true>
{
    typedef long type;
};
template <>
struct __make_signed<unsigned long, true>
{
    typedef long type;
};
template <>
struct __make_signed<long long, true>
{
    typedef long long type;
};
template <>
struct __make_signed<unsigned long long, true>
{
    typedef long long type;
};
}

template <class T>
struct make_signed
{
    typedef typename details::__copy_cv<T, typename details::__make_signed<std::remove_cv_t<T>>::type>::type type;
};
template <class T>
using make_signed_t = typename make_signed<T>::type;

// TODO implem: make_unsigned for enum
namespace details {
template <class T, bool = std::is_integral_v<T> || std::is_enum_v<T>>
struct __make_unsigned{};

template <>
struct __make_unsigned<char, true>
{
    typedef unsigned char type;
};
template <>
struct __make_unsigned<signed char, true>
{
    typedef unsigned char type;
};
template <>
struct __make_unsigned<unsigned char, true>
{
    typedef unsigned char type;
};
template <>
struct __make_unsigned<short, true>
{
    typedef unsigned short type;
};
template <>
struct __make_unsigned<unsigned short, true>
{
    typedef unsigned short type;
};
template <>
struct __make_unsigned<int, true>
{
    typedef unsigned int type;
};
template <>
struct __make_unsigned<unsigned int, true>
{
    typedef unsigned int type;
};
template <>
struct __make_unsigned<long, true>
{
    typedef unsigned long type;
};
template <>
struct __make_unsigned<unsigned long, true>
{
    typedef unsigned long type;
};
template <>
struct __make_unsigned<long long, true>
{
    typedef unsigned long long type;
};
template <>
struct __make_unsigned<unsigned long long, true>
{
    typedef unsigned long long type;
};
}

template <class T>
struct make_unsigned
{
    typedef typename details::__copy_cv<T, typename details::__make_unsigned<std::remove_cv_t<T>>::type>::type type;
};
template <class T>
using make_unsigned_t = typename make_unsigned<T>::type;
}
