/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/_conditional.h>
#include <def/type_traits/_decay.h>
#include <def/type_traits/_enable_if.h>
#include <def/type_traits/_type_identity.h>

#include <def/type_traits/_is_invocable.h>

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

// remove_cvref
template <class T>
struct remove_cvref
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template <class T>
using remove_cvref_t = remove_cvref<T>::type;

// common_type
// primary template (used for zero types)
template <class...>
struct common_type
{};

// one type
template <class T>
struct common_type<T> : common_type<T, T>
{};

namespace detail {
template <class...>
using void_t = void;

template <class T1, class T2>
using conditional_result_t = decltype(false ? std::declval<T1>() : std::declval<T2>());

template <class, class, class = void>
struct decay_conditional_result
{};
template <class T1, class T2>
struct decay_conditional_result<T1, T2, void_t<conditional_result_t<T1, T2>>> : std::decay<conditional_result_t<T1, T2>>
{};

template <class T1, class T2, class = void>
struct common_type_2_impl : decay_conditional_result<const T1&, const T2&>
{};

// C++11 implementation:
// template<class, class, class = void>
// struct common_type_2_impl {};

template <class T1, class T2>
struct common_type_2_impl<T1, T2, void_t<conditional_result_t<T1, T2>>> : decay_conditional_result<T1, T2>
{};
}

// two types
template <class T1, class T2>
struct common_type<T1, T2> :
    std::conditional<std::is_same<T1, typename std::decay<T1>::type>::value &&
                         std::is_same<T2, typename std::decay<T2>::type>::value,
                     detail::common_type_2_impl<T1, T2>,
                     common_type<typename std::decay<T1>::type, typename std::decay<T2>::type>>::type
{};

// 3+ types
namespace detail {
template <class AlwaysVoid, class T1, class T2, class... R>
struct common_type_multi_impl
{};
template <class T1, class T2, class... R>
struct common_type_multi_impl<void_t<typename common_type<T1, T2>::type>, T1, T2, R...> :
    common_type<typename common_type<T1, T2>::type, R...>
{};
}
template <class T1, class T2, class... R>
struct common_type<T1, T2, R...> : detail::common_type_multi_impl<void, T1, T2, R...>
{};
template <class... T>
using common_type_t = typename common_type<T...>::type;

// common_reference
// TODO implem: common_reference
// template <class... T>
// struct common_reference;
// template <class... T>
// using common_reference_t = std::common_reference<T...>::type;

// TODO implem: basic_common_reference (requries common_reference)
// basic_common_reference
// template <class T, class U, template <class> class TQual, template <class> class UQual>
// struct basic_common_reference
//{};

// underlying_type
namespace detail {
template <class T, bool = is_enum<T>::value>
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

/// void_t
template <class...>
using void_t = void;

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
