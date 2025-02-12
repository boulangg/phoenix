/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

//#include <def/type_traits/_conditional.h>
//#include <def/type_traits/_decay.h>
//#include <def/type_traits/_enable_if.h>
//#include <def/type_traits/_is_const.h>
//#include <def/type_traits/_is_volatile.h>
//#include <def/type_traits/_type_identity.h>
//#include <def/type_traits/_void.h>

//#include <def/type_traits/_is_invocable.h>

#include <def/type_traits/_common_type.h>
#include <def/type_traits/_remove_cvref.h>
#include <def/type_traits/_common_reference.h>

namespace std {
// aligned_storage (deprecated)
// TODO implem: aligned_storage (deprecated)
// namespace {
// constexpr std::size_t DEFAULT_ALIGNMENT = 4;
//}
// template <std::size_t Len, std::size_t Align = DEFAULT_ALIGNMENT>
// struct aligned_storage
//{
//    struct type
//    {
//        alignas(Align) unsigned char data[Len];
//    };
//} __attribute__((deprecated));
// template <std::size_t Len, std::size_t Align = DEFAULT_ALIGNMENT>
// using aligned_storage_t = typename aligned_storage<Len, Align>::type;

// aligned_union (deprecated)
// TODO implem: aligned_union (deprecated)
// template <std::size_t Len, class... Types>
// struct aligned_union
//{
//    static constexpr std::size_t alignment_value = std::max({alignof(Types)...});
//
//    struct type
//    {
//        alignas(alignment_value) char _s[std::max({Len, sizeof(Types)...})];
//    };
//} __attribute__((deprecated));
// template <std::size_t Len, class... Types>
// using aligned_union_t = typename aligned_union<Len, Types...>::type;

// underlying_type
namespace detail {
template <class T, bool = std::is_enum<T>::value>
struct __underlying_type_impl;
template <class T>
struct __underlying_type_impl<T, false>
{};
template <class T>
struct __underlying_type_impl<T, true>
{
    typedef __underlying_type(T) type;
};
}
template <class T>
struct underlying_type : detail::__underlying_type_impl<T>
{};
template <class T>
using underlying_type_t = typename underlying_type<T>::type;

/// unwrap_reference
template <class T>
struct unwrap_reference
{
    using type = T;
};
template <class U>
struct unwrap_reference<std::reference_wrapper<U>>
{
    using type = U&;
};
template <class T>
using unwrap_reference_t = unwrap_reference<T>::type;

template <class T>
struct unwrap_ref_decay : std::unwrap_reference<std::decay_t<T>>
{};
template <class T>
using unwrap_ref_decay_t = unwrap_ref_decay<T>::type;

}
