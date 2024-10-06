/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/_is_same.h>
#include <def/type_traits/_is_base_of.h>
#include <def/type_traits/_is_invocable.h>
#include <def/type_traits/logical_operations.h>

namespace std {
// is_convertible
namespace detail {
template <class From, class To>
auto test_implicitly_convertible(int)
    -> decltype(void(std::declval<void (&)(To)>()(std::declval<From>())), std::true_type{});
template <class, class>
auto test_implicitly_convertible(...) -> std::false_type;

template <class T>
auto test_returnable(int) -> decltype(void(static_cast<T (*)()>(nullptr)), std::true_type{});
template <class>
auto test_returnable(...) -> std::false_type;
} // namespace detail

template <class From, class To>
struct is_convertible :
    std::integral_constant<bool, (decltype(detail::test_returnable<To>(0))::value &&
                                  decltype(detail::test_implicitly_convertible<From, To>(0))::value) ||
                                     (std::is_void<From>::value && std::is_void<To>::value)>
{};
template <class From, class To>
constexpr bool is_convertible_v = is_convertible<From, To>::value;

template <class From, class To>
struct is_nothrow_convertible : std::conjunction<std::is_void<From>, std::is_void<To>>
{};

template <class From, class To>
    requires requires {
        static_cast<To (*)()>(nullptr);
        {
            std::declval<void (&)(To) noexcept>()(std::declval<From>())
        } noexcept;
    }
struct is_nothrow_convertible<From, To> : std::true_type
{};
template <class From, class To>
constexpr bool is_nothrow_convertible_v = is_nothrow_convertible<From, To>::value;

// is_layout_compatible
// TODO implem: is_layout_compatible
// template <class T, class U>
// struct is_layout_compatible;
// template <class T, class U>
// inline constexpr bool is_layout_compatible_v = is_layout_compatible<T, U>::value;

// is_pointer_interconvertible_base_of
// TODO implem: is_pointer_interconvertible_base_of
// template <class Base, class Derived>
// struct is_pointer_interconvertible_base_of;
// template <class Base, class Derived>
// inline constexpr bool is_pointer_interconvertible_base_of_v = is_pointer_interconvertible_base_of<Base,
// Derived>::value;
}
