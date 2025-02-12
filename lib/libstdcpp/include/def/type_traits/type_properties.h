/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <cstddef>

#include <def/utility_declval.h>

#include <def/type_traits/_is_const.h>
#include <def/type_traits/_is_volatile.h>
#include <def/type_traits/integral_constant.h>
#include <def/type_traits/type_categories.h>
#include <def/type_traits/type_relationship.h>
#include <def/type_traits/misc_transformations.h>

namespace std {
template <class T>
struct is_trivial : std::bool_constant<__is_trivial(T)>
{};
template <class T>
inline constexpr bool is_trivial_v = is_trivial<T>::value;

template <class T>
struct is_trivially_copyable : std::bool_constant<__has_trivial_copy(T)>
{};
template <class T>
inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

template <class T>
struct is_standard_layout : std::bool_constant<__is_standard_layout(T)>
{};
template <class T>
inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

// TODO implem: has_unique_object_representations
// template <class T>
// struct has_unique_object_representations;
// template <class T>
// constexpr bool has_unique_object_representations_v =
//    has_unique_object_representations<T>::value;

template <class T>
struct is_empty : std::bool_constant<__is_empty(T)>
{};
template <class T>
inline constexpr bool is_empty_v = is_empty<T>::value;

namespace detail {
template <class T>
std::true_type detect_is_polymorphic(decltype(dynamic_cast<const volatile void*>(static_cast<T*>(nullptr))));
template <class T>
std::false_type detect_is_polymorphic(...);
}

template <class T>
struct is_polymorphic : decltype(detail::detect_is_polymorphic<T>(nullptr))
{};
template <class T>
constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

template <class T>
struct is_abstract : std::bool_constant<__is_abstract(T)>
{};
template <class T>
inline constexpr bool is_abstract_v = is_abstract<T>::value;

template <class T>
struct is_final : std::bool_constant<__is_final(T)>
{};
template <class T>
inline constexpr bool is_final_v = is_final<T>::value;

template <class T>
struct is_aggregate : std::bool_constant<__is_aggregate(T)>
{};
template <class T>
inline constexpr bool is_aggregate_v = is_aggregate<T>::value;

// TODO implem: is_implicit_lifetime
// template <class T>
// struct is_implicit_lifetime;
// template <class T>
// constexpr bool is_implicit_lifetime_v = is_implicit_lifetime<T>::value;

namespace detail {
template <typename T, bool = std::is_arithmetic_v<T>>
struct is_signed : std::integral_constant<bool, T(-1) < T(0)>
{};
template <typename T>
struct is_signed<T, false> : std::false_type
{};
}
template <typename T>
struct is_signed : detail::is_signed<T>::type
{};
template <class T>
constexpr bool is_signed_v = is_signed<T>::value;

namespace detail {
template <typename T, bool = std::is_arithmetic_v<T>>
struct is_unsigned : std::integral_constant<bool, T(0) < T(-1)>
{};
template <typename T>
struct is_unsigned<T, false> : std::false_type
{};
}
template <typename T>
struct is_unsigned : detail::is_unsigned<T>::type
{};
template <class T>
constexpr bool is_unsigned_v = is_unsigned<T>::value;

template <class T>
struct is_bounded_array : std::false_type
{};
template <class T, std::size_t N>
struct is_bounded_array<T[N]> : std::true_type
{};
template <class T>
constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

template <class T>
struct is_unbounded_array : std::false_type
{};
template <class T>
struct is_unbounded_array<T[]> : std::true_type
{};
template <class T>
constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

template <typename E>
    struct is_scoped_enum : std::bool_constant < requires
{
    requires std::is_enum_v<E>;
    requires !std::is_convertible_v<E, std::underlying_type_t<E>>;
} > {};
template <class E>
constexpr bool is_scoped_enum_v = is_scoped_enum<E>::value;
}
