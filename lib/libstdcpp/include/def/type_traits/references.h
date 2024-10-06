/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/_type_identity.h>

namespace std {
namespace detail {
template <class T> // Note that “cv void&” is a substitution failure
auto try_add_lvalue_reference(int) -> type_identity<T&>;
template <class T> // Handle T = cv void case
auto try_add_lvalue_reference(...) -> type_identity<T>;

template <class T>
auto try_add_rvalue_reference(int) -> type_identity<T&&>;
template <class T>
auto try_add_rvalue_reference(...) -> type_identity<T>;
} // namespace detail

template <class T>
struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0))
{};
template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template <class T>
struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0))
{};
template <class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template <class T>
struct remove_reference
{
    typedef T type;
};
template <class T>
struct remove_reference<T&>
{
    typedef T type;
};
template <class T>
struct remove_reference<T&&>
{
    typedef T type;
};
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
}
