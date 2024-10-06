/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/_type_identity.h>
#include <def/type_traits/references.h>

namespace std {
// Pointers
template <class T>
struct remove_pointer
{
    typedef T type;
};
template <class T>
struct remove_pointer<T*>
{
    typedef T type;
};
template <class T>
struct remove_pointer<T* const>
{
    typedef T type;
};
template <class T>
struct remove_pointer<T* volatile>
{
    typedef T type;
};
template <class T>
struct remove_pointer<T* const volatile>
{
    typedef T type;
};
template <class T>
using remove_pointer_t = typename remove_pointer<T>::type;

namespace detail {
template <class T>
auto try_add_pointer(int) -> type_identity<typename std::remove_reference<T>::type*>; // usual case

template <class T>
auto try_add_pointer(...) -> type_identity<T>; // unusual case (cannot form std::remove_reference<T>::type*)
} // namespace detail

template <class T>
struct add_pointer : decltype(detail::try_add_pointer<T>(0))
{};
template <class T>
using add_pointer_t = typename add_pointer<T>::type;
}
