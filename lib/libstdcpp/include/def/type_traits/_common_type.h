/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/_conditional.h>
#include <def/type_traits/_decay.h>
#include <def/type_traits/_remove_cvref.h>
#include <def/type_traits/_void.h>
#include <def/utility_declval.h>

namespace std {
// Declaration
template <class...>
struct common_type;

// Helper type
template <class... T>
using common_type_t = typename common_type<T...>::type;

namespace details {
template <class T1, class T2>
using __conditional_result_t = decltype(false ? std::declval<T1>() : std::declval<T2>());

// 2-4) if std::decay<decltype(false ? std::declval<CR1>() : std::declval<CR2>())>::type is a valid type,
// where CR1 and CR2 are const std::remove_reference_t<T1>&and const std::remove_reference_t<T2>&respectively,
// the member type denotes that type
template <class T1, class T2, class = void>
struct __common_type_2_4_impl
{};

template <class T1, class T2>
struct __common_type_2_4_impl<T1, T2, std::void_t<__conditional_result_t<const T1&, const T2&>>>
{
    using type = std::remove_cvref_t<__conditional_result_t<const T1&, const T2&>>;
};

// 2-3) if std::decay<decltype(false ? std::declval<T1>() : std::declval<T2>())>::type is a valid type,
// the member type denotes that type
template <class T1, class T2, class = void>
struct __common_type_2_3_impl : __common_type_2_4_impl<T1, T2>
{};

template <class T1, class T2>
struct __common_type_2_3_impl<T1, T2, std::void_t<decltype(true ? std::declval<T1>() : std::declval<T2>())>>
{
    using type = std::decay_t<decltype(true ? std::declval<T1>() : std::declval<T2>())>;
};
}

// 0) If sizeof...(T) is zero, there is no member type
template <class... T>
struct common_type
{};

// 1) If sizeof...(T) is one (i.e., T... contains only one type T0)
template <class T>
struct common_type<T> : common_type<T, T>
{};

// 2) If sizeof...(T) is two (i.e., T... contains exactly two types T1 and T2)
// 2-1) If applying std::decay to at least one of T1 and T2 produces a different type, the member type names the same
// type as std::common_type<std::decay<T1>::type, std::decay<T2>::type>::type, if it exists
template <class T1, class T2>
struct common_type<T1, T2> :
    std::conditional_t<std::is_same_v<T1, std::decay_t<T1>> && std::is_same_v<T2, std::decay_t<T2>>,
                       details::__common_type_2_3_impl<T1, T2>, std::common_type<std::decay_t<T1>, std::decay_t<T2>>>
{};

// 3+) If sizeof...(T) is greater than two (i.e., T... consists of the types T1, T2, R...)
// if std::common_type<T1, T2>::type exists, the member type denotes std::common_type<typename std::common_type<T1,
// T2>::type, R...>::type if such a type exists
namespace detail {
template <class AlwaysVoid, class T1, class T2, class... R>
struct common_type_multi_impl
{};
template <class T1, class T2, class... R>
struct common_type_multi_impl<std::void_t<typename std::common_type_t<T1, T2>>, T1, T2, R...> :
    std::common_type<typename std::common_type_t<T1, T2>, R...>
{};
}
template <class T1, class T2, class... R>
struct common_type<T1, T2, R...> : detail::common_type_multi_impl<void, T1, T2, R...>
{};
}
