/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <def/type_traits/_common_type.h>
#include <def/type_traits/_copy_cv_ref.h>
#include <def/type_traits/_enable_if.h>
#include <def/type_traits/references.h>
#include <def/type_traits/type_relationship.h>

namespace std {
// Declaration
template <class...>
struct common_reference;

// Helper type
template <class... T>
using common_reference_t = typename common_reference<T...>::type;

// basic_common_reference
template <class, class, template <class> class, template <class> class>
struct basic_common_reference
{};

namespace details {
template <class T1, class T2>
using __decltype_res = decltype(false ? std::declval<T1>() : std::declval<T2>());

// Simple common reference types
template <class T1, class T2, class X = std::remove_reference_t<T1>, class Y = std::remove_reference_t<T2>>
struct __simple_common_reference;

template <class T1, class T2>
using __simple_common_reference_t = typename __simple_common_reference<T1, T2>::type;

template <class X, class Y>
using __combined_decltype = __decltype_res<__copy_cv_t<X, Y>&, __copy_cv_t<Y, X>&>;

// If T1 is cv1 X& and T2 is cv2 Y& (i.e., both are lvalue reference types): their simple common reference type is
// decltype(false? std::declval<cv12 X&>() : std::declval<cv12 Y&>()), where cv12 is the union of cv1 and cv2, if that
// type exists and is a reference type
template <class T1, class T2, class X, class Y>
    requires 
        requires { typename __combined_decltype<X, Y>; } && 
        std::is_reference_v<__combined_decltype<X, Y>>
struct __simple_common_reference<T1&, T2&, X, Y>
{
    using type = __combined_decltype<X, Y>;
};

template <class X, class Y>
using __common_ref_C = remove_reference_t<__simple_common_reference_t<X&, Y&>>&&;

// If T1 and T2 are both rvalue reference types : if the simple common reference type of T1& and T2& (determined
// according to the previous bullet) exists, then let C denote that type's corresponding rvalue reference type. If
// std::is_convertible_v<T1, C> and std::is_convertible_v<T2, C> are both true, then the simple common reference type of
// T1 and T2 is C
template <class T1, class T2, class X, class Y>
    requires 
        requires { typename __common_ref_C<X, Y>; } && 
        std::is_convertible_v<T1&&, __common_ref_C<X, Y>> &&
        std::is_convertible_v<T2&&, __common_ref_C<X, Y>>
struct __simple_common_reference<T1&&, T2&&, X, Y>
{
    using type = __common_ref_C<X, Y>;
};

template <class X, class Y>
using __common_ref_D = __simple_common_reference_t<const X&, Y&>;

// If one of the two types must be an lvalue reference type A&and the other must be an rvalue reference type B
// && (A and B might be cv - qualified).Let D denote the simple common reference type of A&and B const&, if any.If D
// exists and std::is_convertible_v<B&&, D> is true, then the simple common reference type is D
template <class T1, class T2, class X, class Y>
    requires 
        requires { typename __common_ref_D<X, Y>; } && 
        std::is_convertible_v<T1&&, __common_ref_D<X, Y>>
struct __simple_common_reference<T1&&, T2&, X, Y>
{
    using type = __common_ref_D<X, Y>;
};

template <class T1, class T2, class X, class Y>
struct __simple_common_reference<T1&, T2&&, X, Y> : __simple_common_reference<T2&&, T1&>
{};

// Otherwise, there's no simple common reference type.
template <class T1, class T2, class X, class Y>
struct __simple_common_reference
{};

// 2-X)
template <class T1, class T2>
struct __common_reference_2_4_impl
{};
template <class T1, class T2>
struct __common_reference_2_3_impl : __common_reference_2_4_impl<T1, T2>
{};
template <class T1, class T2>
struct __common_reference_2_2_impl : __common_reference_2_3_impl<T1, T2>
{};
template <class T1, class T2>
struct __common_reference_2_1_impl : __common_reference_2_2_impl<T1, T2>
{};

// 2-1) Let type S be the simple common reference type of T1 and T2 (as defined below). The member type type names S if
// all of the conditions below are satisfied:
// - T1 and T2 are both reference types
// - S is well-formed
// - std::is_convertible_v<std::add_pointer_t<T1>, std::add_pointer_t<S>> and
// std::is_convertible_v<std::add_pointer_t<T2>, std::add_pointer_t<S>> are true
template <class T1, class T2>
    requires std::is_reference_v<T1> && std::is_reference_v<T2> &&
             requires { typename __simple_common_reference_t<T1, T2>; } &&
             std::is_convertible_v<std::add_pointer_t<T1>, std::add_pointer_t<__simple_common_reference_t<T1, T2>>> &&
             std::is_convertible_v<std::add_pointer_t<T2>, std::add_pointer_t<__simple_common_reference_t<T1, T2>>>
struct __common_reference_2_1_impl<T1, T2>
{
    using type = __simple_common_reference_t<T1, T2>;
};

// 2-2) if std::basic_common_reference<std::remove_cvref_t<T1>, std::remove_cvref_t<T2>, T1Q, T2Q>::type exists, where
// TiQ is a unary alias template such that TiQ<U> is U with the addition of Ti's cv- and reference qualifiers, then the
// member type type names that type
template <class _Tp>
struct __xref
{
    template <class _Up>
    using __apply = __copy_cvref_t<_Tp, _Up>;
};

template <class _Tp, class _Up>
using __basic_common_reference_t =
    typename std::basic_common_reference<remove_cvref_t<_Tp>, remove_cvref_t<_Up>, __xref<_Tp>::template __apply,
                                    __xref<_Up>::template __apply>::type;

template <class T1, class T2>
    requires requires {
        typename __basic_common_reference_t<T1, T2>;
    }
struct __common_reference_2_2_impl<T1, T2>
{
    using type = __basic_common_reference_t<T1, T2>;
};

// 2-3) if decltype(false? val<T1>() : val<T2>()), where val is a function template template<class T> T val();, is a
// valid type, then the member type type names that type
template <class T1, class T2>
    requires requires { typename __decltype_res<T1, T2>; }
struct __common_reference_2_3_impl<T1, T2>
{
    using type = __decltype_res<T1, T2>;
};

// 2-4) if std::common_type_t<T1, T2> is a valid type, then the member type type names that type
template <class T1, class T2>
    requires requires { typename common_type<T1, T2>; }
struct __common_reference_2_4_impl<T1, T2>
{
    using type = common_type<T1, T2>::type;
};
}

// 0) If sizeof...(T) is zero
template <>
struct common_reference<>
{};

// 1) If sizeof...(T) is one (i.e., T... contains only one type T0), the member type names the same type as T0.
template <class T0>
struct common_reference<T0>
{
    using type = T0;
};

// 2) If sizeof...(T) is two (i.e., T... contains two types T1 and T2)
template <class T1, class T2>
struct common_reference<T1, T2> : details::__common_reference_2_1_impl<T1, T2>
{};

// 3) If sizeof...(T) is greater than two (i.e., T... consists of the types T1, T2, R...), then if
// std::common_reference_t<T1, T2> exists, the member type denotes std::common_reference_t<std::common_reference_t<T1,
// T2>, R...> if such a type exists. In all other cases, there is no member type.
template <class T1, class T2, class V, class... R>
    requires requires { typename std::common_reference_t<T1, T2>; }
struct common_reference<T1, T2, V, R...> : common_reference<std::common_reference_t<T1, T2>, V, R...>
{};

// Otherwise, there shall be no member `type`.
template <class...>
struct common_reference
{};
}
